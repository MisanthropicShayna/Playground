@name Origin Alternator Transmitter
@inputs Entity:entity IdleVec:vector Button:number
@persist Target:vector
@outputs Origin:vector
Target = Entity:pos()

if(Button == 1){
    Origin = Target   
    timer("delay", 30)
    if(clk("delay")){
        Origin = IdleVec    
    }
} 
else{
    Origin = IdleVec   
}
