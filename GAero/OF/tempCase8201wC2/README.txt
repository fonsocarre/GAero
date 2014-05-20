La malla se guarda en constant/polyMesh.
Ahí tienes (que te interesen):
	- faces: nodos pertenecientes a cada face
	- points: coordenadas de los nodos. En mi malla interesa "x" y "z".
				"y" sólo toma 0 y -1
	+ carpeta sets:
		- PROFILE: faces pertenecientes al perfil
		- zeroVolumeCells: sin comentarios
		- nonOrthoFaces: ídem

!!!!!!! TODOS LOS INDICES EMPIEZAN EN 0, es decir, la primera face es la 0.

Aquí hay una explicación del todo el formato: http://www.openfoam.org/docs/user/mesh.php