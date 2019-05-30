@name Hydrogen Oxygen Controller
@inputs Oxygen Oxygen_Max Hydrogen Hydrogen_Max Energy
@outputs HCompress OCompress

if(Oxygen < 3000 & Energy > 10000 | Oxygen < Oxygen_Max){
    OCompress = 1
}


if(Hydrogen < 3000 & Energy > 10000 | Hydrogen < Hydrogen_Max){
    HCompress = 1
}

if(Oxygen == Oxygen_Max){
    OCompress = 0   
}
if(Hydrogen == Hydrogen_Max){
    HCompress = 0   
}
