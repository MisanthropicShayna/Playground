@name Regulate Gas Compressors
@inputs H HMax O OMax E
@outputs CH CO

if(Hydrogen == Hydrogen_Max | Energy <= 1000){
    CH = 0  
}
else{
    CH = 1   
}

if(Oxygen == Oxygen_Max | Energy <= 1000){
    CO = 0
}
else{
    CO = 1
}

