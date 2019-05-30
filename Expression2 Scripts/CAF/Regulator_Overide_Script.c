@name Overide Interface
@inputs Keyboard:string
@persist Condition:number Keyboard_Length:number Password_Chars:string Overide_Status:string
@outputs Overide:number Display:string Kick:number Clear:number
Keyboard_Length = Keyboard:len()
Condition = 1



if(Condition == 1){
	Password_Chars = ""
	for(I=0,Keyboard_Length,1){
		Password_Chars  = Password_Chars + "*"
	}
	Display = Display + ">> " + Password_Chars + " <<"
}

if(Keyboard == "1:68217700\n"){
	Display = Display + "\n Reactor overide activated."
	Overide = 1
}
elseif(Keyboard == "2:68207700\n"){
	Overide_Status = "\nAbort Code Valid. Reactor overide disabled."
	Overide = 0
}
else
	Clear = 0
	Kick = 0
	timer("delay", 100)
	if(clk("delay")){
		Overide = 0
	}
}