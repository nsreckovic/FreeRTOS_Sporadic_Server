// Serial.print()
void (*debug_msg)(char *);
void (*debug_msgln)(char *);
void (*debug_num)(double);
void (*debug_numln)(double);

// Periodic function
void (*vPeriodicFunc)(void *);

// Aperiodic function
void (*vAperiodicFunc)(void *);



// Serial.print()
void set_debug_msg( void(*print_msg)(char *) ){
    debug_msg = print_msg;
}
void set_debug_msgln( void(*print_msgln)(char *) ){
    debug_msgln = print_msgln;
}
void set_debug_num( void(*print_num)(double) ){
    debug_num = print_num;
}
void set_debug_numln( void(*print_numln)(double) ){
    debug_numln = print_numln;
}

// Periodic function
void set_vPeriodicFunc( void(*vPeriodic_Func)(void *) ){
    vPeriodicFunc = vPeriodic_Func;
}

// Aperiodic function
void set_vAperiodicFunc( void(*vAperiodic_Func)(void *) ){
    vAperiodicFunc = vAperiodic_Func;
}