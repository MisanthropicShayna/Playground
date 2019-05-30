@name Regulate Electrolyzer
@inputs H2O D2O E
@outputs Activate

if(D2O < 10000 & H2O > 20000 & Energy > 50000){
    Activate = 1   
}
elseif(D2O >= 20000){
    Activate = 0  
}
