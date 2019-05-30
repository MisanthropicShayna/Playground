@name Detector
@inputs Detect:entity Arm:number In_Armed:number
@outputs Detonate:number Out_Armed:number
Armed = In_Armed
Target_Vector = Detect:pos()
if(Arm == 1){
    timer("wait", 500)
    if(clk("wait")){
        Armed = 1
    }   
}
 
if(Target_Vector[1] == 0 & Target_Vector[2] == 0 & Target_Vector[3] == 0){
    Detonate = 0
}
else{
    if(Armed == 1){
        Detonate = 1 
          
    } 
}


Out_Armed = Armed
