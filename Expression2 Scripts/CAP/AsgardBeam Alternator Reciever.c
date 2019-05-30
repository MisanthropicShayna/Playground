@name Origin Alternator
@inputs IdleVec:vector Origin1:vector Origin2:vector
@outputs Origin:vector Send:number
if(Origin1 != IdleVec){
    Origin = Origin1
    Send = 1
}
elseif(Origin2 != IdleVec){
    Origin = Origin2   
    Send = 1
}
else{
    Send = 0   
}
