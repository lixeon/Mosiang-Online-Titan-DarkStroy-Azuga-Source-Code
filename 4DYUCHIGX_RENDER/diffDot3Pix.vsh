; Constant registers 
; c0-c3 world space transposed
; c8-c11 world * view * proj
; c12 - Light Direction (In World Space)
; c33 - four constants with 0.5f

; c24 - camera direction
; c25 - tex reflect matrix


;
; Input registers
; v0 - Position
; v3 - Normal
; v7 - Texture 
; v8 - Tangent
vs.1.1

m4x4 oPos, v0, c8

m3x3 r5, v8, c0 ; generate world tangent U
m3x3 r7, v3, c0 ; generate world normal W
m3x3 oT2.xy, v3, c25 ; normal to tex coord for sphere map


; Cross product
; generate world binormal V
mul r0, r5.zxyw, -r7.yzxw;
mad r6, r5.yzxw, -r7.zxyw,-r0;


;;;;;;;;;;;;;;;;;;
; per pixel
;;;;;;;;;;;;;;;;;;
;transform the light vector with U, V, W
dp3 r8.x, r5, -c12		; u r5
dp3 r8.y, r6, -c12		; v r6
dp3 r8.z, r7, -c12		; w r7

; light -> oD0
mad oD0.xyz, r8.xyz, c33, c33 ; multiply a half to bias, then add half

;;;;;;;;;;;;;;;;;;
; specular
;;;;;;;;;;;;;;;;;;
; position in world space
m4x4 r2, v0, c0			

; get a vector toward the camera/eye V
add r2, -r2, c24

; normalize eye vector V
dp3 r11.x,  r2.xyz, r2.xyz	
rsq r11.xyz, r11.x			
mul r2.xyz, r2.xyz, r11.xyz	

add r2.xyz, r2.xyz, -c12		; get half angle
	
; normalize half angle
dp3 r11.x, r2.xyz, r2.xyz		
rsq r11.xyz, r11.x			
mul r2.xyz, r2.xyz, r11.xyz	

; transform the half angle vector into texture space
dp3 r8.x,r5,r2
dp3 r8.y,r6,r2
dp3 r8.z,r7,r2

; half vector -> oD1
mad oD1.xyz, r8.xyz, c33, c33 ; multiply by a half to bias, then add half

mov oT0.xy, v7.xy
mov oT1.xy, v7.xy




