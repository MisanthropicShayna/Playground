@name Naq_Bomb Inputter
@inputs Det_Code:string Abrt_Code:string Action:number RL_Detcode:string RL_Abcode:string Countdown:number
@outputs Detonate:number Detonate_Code:string Abort_Code:string Information:string Abort:number Debug:string

Status = 0
Debug = "Detonation Code In: " + Det_Code + "\nDetonation Code Out: " + Detonate_Code + "\nDetonate: " + toString(Detonate) + "\nAbort: " + toString(Abort)
Detonate_Code = Det_Code
Abort_Code = Det_Code

if(Det_Code == RL_Detcode){
    Information = " > " + Det_Code + " <\nValid Detonation Code"
    Status = 1
}
elseif(Det_Code == RL_Abcode){
    Information = " > " + Det_Code + " <\nValid Abort Code"  
    Status = 2
}
elseif(Det_Code != RL_Detcode){
    Information = " > " + Det_Code + " <\nInvalid Detonation Code"   
    Ststus = 3
}

if(Action == 1 & Status == 1){
   Information = " > " + Det_Code + " <\nNuclear Warhead Armed. Nuclear reaction immenent.\nExplosion In: " + toString(Countdown)
   Detonate = 1
   Abort = 0 
     
}
elseif(Action == 1 & Status == 2){
    Information = " > " + Det_Code + " <\nNuclear Explosion Aborted"
    Abort = 1
    Detonate = 0   
}



