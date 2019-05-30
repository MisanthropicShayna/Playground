@name Regulate Hydrogen Cell
@inputs H HMax O OMax
@outputs Activate

if(H == HMax & O == OMax){
	Activate = 1
}
else{
	Activate = 0
}