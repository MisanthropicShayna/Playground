@name Detector
@inputs Detect:entity
@outputs Detonate:number

Target_Vector = Target:pos() 
if(Target_Vector[1] == 0 & Target_Vector[2] == 0 & Target_Vector[3] == 0){
    Activate = 0
    Arm = 0
}
else{
    Activate = 1   
    Arm = 1
}
