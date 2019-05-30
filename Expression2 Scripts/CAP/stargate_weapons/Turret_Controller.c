@name Turret Controller
@inputs Entity_Vector:vector Idle_Vector:vector Fire_Button:number
@outputs Turret_Vector:vector Turret_Fire:number

NoTarget = 1

if(Entity_Vector[1] == 0 & Entity_Vector[2] == 0 & Entity_Vector[3] == 0){
    Turret_Vector = Idle_Vector
    NoTarget = 1
}

else{
    Turret_Vector = Entity_Vector
    NoTarget = 0
    
}

if(Fire_Button == 1 & NoTarget == 0){
    Turret_Fire = 1 
}

if(Fire_Button == 0){
    Turret_Fire = 0
}



        


