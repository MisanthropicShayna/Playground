@name Display Entity Information 
@inputs Entity_Vector:vector Entity_Name:string Entity_IsNPC:number
@outputs Entity_Information:string


Position = "------X------\n" + toString(Entity_Vector[1]) + "\n------Y------\n" + toString(Entity_Vector[2]) + "\n------Z-------\n" + toString(Entity_Vector[3])

if(Entity_Vector[1] == 0 & Entity_Vector[2] == 0 & Entity_Vector[3] == 0){
    Entity_Information = "--Target--\n" + "No Target" + "\n\n--Position--\n" + Position
}
else{    
    if(Entity_IsNPC == 1){
        Entity_Information = "--Target--\n" + "NPC" + "\n\n--Position--\n" + Position
    }
    if(Entity_IsNPC == 0){
      Entity_Information = "--Target--\n" + Entity_Name + "\n\n--Position--\n" + Position
    }
}




