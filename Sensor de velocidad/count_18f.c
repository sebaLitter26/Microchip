#define BYTE_PTR(x) &(int8 *)(x) 

#define COUNT_DELAY ((int16)(getenv("CLOCK") / 80000)) 

// ASM definitions 
#define W  0 
#define F  1 

// Status Register Bits 
#define Z  2 
#define C  0 

// Register addresses (18F) 
#byte INDF0  = 0xFEF 
#byte STATUS = 0xFD8 
#byte FSR0H  = 0xFEA 
#byte FSR0L  = 0xFE9 

//--------------------------------------- 

int16 count(int16 ccs_pin, int16 period)  
{ 
int8 const bitmask_table[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; 
int16 io_port;    
int8  bitmask; 
int16 R0; 
int16 R1; 
int16 R2; 
int16 R3; 
int32 loop_count; 

// These variables are used in the ASM code 
// and must be located in the same RAM bank. 
#locate R0      = 0x30 
#locate R1      = 0x32 
#locate R2      = 0x34 
#locate R3      = 0x36 
#locate io_port = 0x38  
#locate bitmask = 0x3A  // *** Changed to 0x3A for 18F version *** 

// Extract the Port address and bitmask from 
// the CCS pin number. 
io_port = ccs_pin >> 3; 
bitmask = bitmask_table[ccs_pin & 7]; 

// Set the pin to be an input pin. 
output_float(ccs_pin); 

// Calculate the number of loop iterations to do. 
loop_count = _mul(period, COUNT_DELAY); 

// Put loop_count into R0 (msb), R2+1 (mid), R2 (lsb) 
R0 = make8(loop_count, 2);  // msb 
(int8)R2 = make8(loop_count, 1);  // Get mid into  R2 
R2 <<= 8;  // Move mid into R2+1 
(int8)R2 = make8(loop_count, 0); // lsb 

#asm    
    movf    io_port, W      ; Get port lsb 
    movwf   FSR0L 
    movf    BYTE_PTR(io_port) +1, W   ; Get port msb 
    movwf   FSR0H 
    incf    R0, F              ; Bump up high and mid for dec 
    incf    BYTE_PTR(R2) +1, F 
    clrf    R1                 ; Zero counter 
    clrf    BYTE_PTR(R1) +1 
    movf    INDF0, W  
    andwf   bitmask, W         ; Isolate it 
    movwf   R3                 ; Save starting state as last 

countloop:                     ; 20 usec loop (at 4 MHz) 
    nop                        ; 1 
    movf    INDF0, W           ; 1 
    andwf   bitmask, W         ; 1 Isolate it 
    movwf   BYTE_PTR(R3) +1    ; 1 Save state 
    xorwf   R3, W              ; 1 Compare with last time 
    andwf   BYTE_PTR(R3) +1, W ; 1 Only count high states 
    xorwf   bitmask, W         ; 1 Flip for next test 
    btfsc   STATUS, Z          ; 1 / 2 
    incf    R1, F              ; 1 / 0 Count pulse 
    btfsc   STATUS, Z          ; 1 / 2 
    incf    BYTE_PTR(R1) +1, F ; 1 / 0 
    movf    BYTE_PTR(R3) +1, W ; 1 Set new last state 
    movwf   R3                 ; 1 
    decf    R2, F              ; 1 Count time 
    btfsc   STATUS, Z          ; 1 / 2 
    decf    BYTE_PTR(R2) +1, F ; 1 / 0 
    btfsc   STATUS, Z          ; 1 / 2 
    decfsz  R0, F              ; 1 / 2 
    goto    countloop          ; 2 / 0 
    movf    R1, W              ; Result to W 
#endasm 

return(R1);  // R1 holds the result 
} 