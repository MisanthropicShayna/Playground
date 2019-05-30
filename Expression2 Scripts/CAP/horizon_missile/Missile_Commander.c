@name Controller
@inputs Keyboard_Input:string Code:string  Clock:number
@outputs Deploy:number Fire:number Ignite:number Monitor1:string Monitor2:string


Monitor1 = "  Commands: Deploy, Ignite, Fire \n\n  Cover_Status: " + toString(Deploy) + "\n  Ignition_Status: " + toString(Ignite) + "\n  Payload_Status: " + toString(Fire) + "\n\n  " + Keyboard_Input 

if(Keyboard_Input == "Deploy:" + Code & Clock == 1){
    Deploy = 1  
}
elseif(Keyboard_Input == "Ignite:" + Code & Clock == 1){
    Ignite = 1
}
elseif(Keyboard_Input == "Fire" & Ignite == 1 & Clock == 1){
    Fire = 1   
}
