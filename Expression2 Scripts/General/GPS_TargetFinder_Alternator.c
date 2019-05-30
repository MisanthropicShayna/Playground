@inputs AimGPS:number AimTarget:number GPS:vector Target:vector
@outputs AimLocation:vector Using:number

if(AimGPS == 1 & AimTarget == 0){
    AimLocation = GPS   
    Using = 2
}
elseif(AimTarget == 1 & AimGPS == 0){
    AimLocation = Target   
    Using = 1
}



