@name Electrolyzer Controller
@inputs Energy:number Water:number Deuterium:number Deuterium_Max:number
@outputs Activate:number

if(Deuterium < Deuterium_Max - 1000 & Water > 4000 & Energy > 4000){
    Activate = 1   
} else{
    Activate = 0
}


