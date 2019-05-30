@name Fusion Reactor Controller
@inputs Water Energy Max_Energy
@outputs Activate:number

if(Water > 10000 & Energy < Max_Energy - 10000){
   Activate = 1
}
else{
    Activate = 0   
}

