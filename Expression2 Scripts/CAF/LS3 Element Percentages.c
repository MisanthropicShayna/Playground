@name Element Level Proceccor
@inputs H HM O OM H2O H2OM D2O D2OM H2OG H2OGM E EM
@outputs EL:string

function string getPercentage(Min:number, Max:number){
    return toString(round((Min/Max)*100)) + "%"
}

EL = " H: " + getPercentage(H, HM) + " [" + H + " / " + HM + "]\n "
EL = EL + "O: " + getPercentage(O, OM) + " [" + O + " / " + OM + "]\n "
EL = EL + "E: " + getPercentage(E, EM) + " [" + E + " / " + EM + "]\n " 
EL = EL + "H2O: " + getPercentage(H2O, H2OM) + " [" + H2O + " / " + H2OM + "]\n "
EL = EL + "D2O: " + getPercentage(D2O, D2OM) + " [" + D2O + " / " + D2OM + "]\n "
EL = EL + "H2OG: " + getPercentage(H2OG, H2OGM) + " [" + H2OG + " / " + H2OGM + "]\n\n " 



