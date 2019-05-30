@name Regulate Steam Turbine
@inputs GH2O
@outputs Activate

if(Steam > 100){
    Activate = 1
}
else{
    Activate = 0   
}
