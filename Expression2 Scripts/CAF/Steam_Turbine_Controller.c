@name Steam Turbine Controller
@inputs Steam Energy
@outputs Activate

if(Steam > 1100 & Energy > 5000){
    Activate = 1   
}
