@name Fuel Cell Controller
@inputs Hydrogen Hydrogen_Max Oxygen Oxygen_Max Water Water_Max
@outputs Activate:number


if(Water < Water_Max & Hydrogen == Hydrogen_Max & Oxygen == Oxygen_Max){
    Activate = 1   
}

if(Hydrogen < 3000 | Oxygen < 3000){
    Activate = 0   
}
