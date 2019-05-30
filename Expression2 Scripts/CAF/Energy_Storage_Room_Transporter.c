@name Energy Storage Room Transporter
@inputs Code_Accepted
@outputs Origin_Position:vector 
@persist
runOnTick(1)
findInSphere(entity():pos(),10)
findIncludeClass("player")
findExcludeClass("npc")
findIncludeClass(owner())
Origin_Position = findResult(1)
