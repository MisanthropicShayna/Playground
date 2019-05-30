@name Regulate Fusion Reactor
@inputs E:number EMax:number H2O:number Overide:number
@outputs Activate:number

function number getPercentage(Val:number, Max:number){
	return round((Val/Max)*100)
}


if(E >= EMax - 500 | Water <= 5000 & Overide == 0){
    Activate = 0   
}
elseif(getPercentage(E, EMax) <= 95 | Overide == 1){
    Activate = 1    
}
