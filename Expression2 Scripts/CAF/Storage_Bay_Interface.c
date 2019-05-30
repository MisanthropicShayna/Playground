@name Storage Bay Interface
@inputs H2O H2OM D2O D2OM H HM O OM Steam SteamM Energy EnergyM
@outputs ScreenText:string


Percentage_Water = round(H2O/H2OM*100)
Percentage_HeavyWater = round(D2O/D2OM*100)
Percentage_Steam = round(Steam/SteamM*100)
Percentage_Oxygen = round(O/OM*100)
Percentage_Hydrogen = round(H/HM*100)
Percentage_Energy = round(Energy/EnergyM*100)


ScreenText = " Energy Level - " + Percentage_Energy:toString() + "%\n"
ScreenText += " Water Level - " + Percentage_Water:toString() + "%\n"
ScreenText += " Deuterium Level - " + Percentage_HeavyWater:toString() + "%\n"
ScreenText += " Steam Level - " + Percentage_Steam:toString() + "%\n"
ScreenText += " Hydrogen Level - " + Percentage_Hydrogen:toString() + "%\n"
ScreenText += " Oxygen Level - " + Percentage_Oxygen:toString() + "%\n"


