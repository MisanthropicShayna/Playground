@name Relay
@inputs In_vector1:vector In_vector2:vector In_vector3:vector In_vector4:vector In_Target:vector
@outputs In_vector1:vector In_vector2:vector In_vector3:vector In_vector4:vector Information:string Out_Target:vector

Information = "  Missile 1 -\n  X: " + toString(In_vector1[1]) + "\n  Y: " + toString(In_vector1[2]) + "\n  Z:  " + toString(In_vector1[3]) + "\n\n  Missile 2 -\n  X: " + toString(In_vector2[1]) + "\n  Y: " + toString(In_vector2[2]) + "\n  Z: " + toString(In_vector2[3]) + "\n\n" + "  Missile 3 -\n  X: " + toString(In_vector3[1]) + "\n  Y: " + toString(In_vector3[2]) + "\n  Z: " + toString(In_vector3[3]) + "\n\n  Missile 4 -\n  X: " + toString(In_vector4[1]) + "\n  Y: " + toString(In_vector4[2]) + "\n  Z: " + toString(In_vector4[3])
