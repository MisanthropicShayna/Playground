@inputs UpLeft:vector UpRight:vector DownLeft:vector DownRight:vector
@outputs Aim:vector Red:number Green:number Blue:number

Cycle = 1

if(Cycle == 1){
  concmd("say Test")   
}

if(clk("Rotation")){
    
    if(Cycle == 1){
        Aim = UpLeft
        Cycle = 2
        timer("Rotation", 1000)
        Red = 255
        Blue = 0
        Green = 0
    }
    if(Cycle == 2){
        Aim = DownRight
        Cycle = 3 
        timer("Rotation", 1000) 
        Red = 0
        Blue = 255
        Green = 0
    }
    if(Cycle == 3){
        Aim = DownLeft
        Cycle = 4
        timer("Rotation", 1000)
        Red = 0
        Blue = 0
        Green = 255
    }
    if(Cycle == 4){
        Aim = UpRight
        Cycle = 1    
        timer("Rotation", 1000)
    }
}

timer("Rotation", 1000)





