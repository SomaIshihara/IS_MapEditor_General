xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 110;
 -30.00000;8.82508;30.00000;,
 30.00000;8.82508;30.00000;,
 30.00000;8.82508;-30.00000;,
 -30.00000;8.82508;-30.00000;,
 -34.00000;-0.20629;-34.00000;,
 34.00000;-0.20629;-34.00000;,
 34.00000;-0.20629;34.00000;,
 -34.00000;-0.20629;34.00000;,
 34.00000;5.73028;-34.00000;,
 30.00000;8.82508;-30.00000;,
 34.00000;5.73028;34.00000;,
 34.00000;-0.20629;-34.00000;,
 34.00000;5.73028;-34.00000;,
 34.00000;-0.20629;-34.00000;,
 -34.00000;-0.20629;-34.00000;,
 -34.00000;5.73028;-34.00000;,
 -30.00000;8.82508;-30.00000;,
 30.00000;8.82508;-30.00000;,
 34.00000;5.73028;34.00000;,
 30.00000;8.82508;30.00000;,
 -30.00000;8.82508;30.00000;,
 -34.00000;5.73028;34.00000;,
 -34.00000;-0.20629;34.00000;,
 34.00000;-0.20629;34.00000;,
 -34.00000;5.73028;-34.00000;,
 -34.00000;-0.20629;-34.00000;,
 -34.00000;5.73028;34.00000;,
 -30.00000;8.82508;-30.00000;,
 0.00000;14.13448;-8.00000;,
 5.65685;14.13448;-5.65685;,
 5.65685;6.63448;-5.65685;,
 0.00000;6.63448;-8.00000;,
 4.24264;14.13448;-4.24264;,
 0.00000;14.13448;-6.00000;,
 0.00000;6.63448;-6.00000;,
 4.24264;6.63448;-4.24264;,
 8.00000;14.13448;0.00000;,
 8.00000;6.63448;0.00000;,
 6.00000;14.13448;0.00000;,
 6.00000;6.63448;0.00000;,
 5.65685;14.13448;5.65685;,
 5.65685;6.63448;5.65685;,
 4.24264;14.13448;4.24264;,
 4.24264;6.63448;4.24264;,
 -0.00000;14.13448;8.00000;,
 -0.00000;6.63448;8.00000;,
 -0.00000;14.13448;6.00000;,
 -0.00000;6.63448;6.00000;,
 -5.65686;14.13448;5.65685;,
 -5.65686;6.63448;5.65685;,
 -4.24264;14.13448;4.24264;,
 -4.24264;6.63448;4.24264;,
 -8.00000;14.13448;-0.00000;,
 -8.00000;6.63448;-0.00000;,
 -6.00000;14.13448;-0.00000;,
 -6.00000;6.63448;-0.00000;,
 -5.65686;14.13448;-5.65685;,
 -5.65686;6.63448;-5.65685;,
 -4.24264;14.13448;-4.24264;,
 -4.24264;6.63448;-4.24264;,
 0.00000;14.13448;-8.00000;,
 0.00000;6.63448;-8.00000;,
 0.00000;14.13448;-6.00000;,
 0.00000;6.63448;-6.00000;,
 4.24264;14.13448;-4.24264;,
 5.65685;14.13448;-5.65685;,
 0.00000;6.63448;-8.00000;,
 5.65685;6.63448;-5.65685;,
 4.24264;6.63448;-4.24264;,
 0.00000;6.63448;-6.00000;,
 6.00000;14.13448;0.00000;,
 8.00000;14.13448;0.00000;,
 8.00000;6.63448;0.00000;,
 6.00000;6.63448;0.00000;,
 4.24264;14.13448;4.24264;,
 5.65685;14.13448;5.65685;,
 5.65685;6.63448;5.65685;,
 4.24264;6.63448;4.24264;,
 -0.00000;14.13448;6.00000;,
 -0.00000;14.13448;8.00000;,
 -0.00000;6.63448;8.00000;,
 -0.00000;6.63448;6.00000;,
 -4.24264;14.13448;4.24264;,
 -5.65686;14.13448;5.65685;,
 -5.65686;6.63448;5.65685;,
 -4.24264;6.63448;4.24264;,
 -6.00000;14.13448;-0.00000;,
 -8.00000;14.13448;-0.00000;,
 -8.00000;6.63448;-0.00000;,
 -6.00000;6.63448;-0.00000;,
 -4.24264;14.13448;-4.24264;,
 -5.65686;14.13448;-5.65685;,
 -5.65686;6.63448;-5.65685;,
 -4.24264;6.63448;-4.24264;,
 0.00000;88.38010;0.00000;,
 25.41430;50.88010;0.00000;,
 25.41430;50.88010;-25.41430;,
 0.00000;13.38009;0.00000;,
 0.00000;13.38009;0.00000;,
 25.41430;50.88010;25.41430;,
 0.00000;88.38010;0.00000;,
 -25.41430;50.88010;0.00000;,
 -25.41430;50.88010;25.41430;,
 -25.41430;50.88010;-25.41430;,
 0.00000;50.88010;-25.41430;,
 25.41430;50.88010;-25.41430;,
 25.41430;50.88010;25.41430;,
 0.00000;50.88010;25.41430;,
 -25.41430;50.88010;-25.41430;,
 -25.41430;50.88010;25.41430;;
 
 58;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,1,10;,
 4;8,10,6,11;,
 4;12,13,14,15;,
 4;12,15,16,17;,
 4;18,19,20,21;,
 4;18,21,22,23;,
 4;24,25,7,26;,
 4;24,26,0,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;29,36,37,30;,
 4;38,32,35,39;,
 4;36,40,41,37;,
 4;42,38,39,43;,
 4;40,44,45,41;,
 4;46,42,43,47;,
 4;44,48,49,45;,
 4;50,46,47,51;,
 4;48,52,53,49;,
 4;54,50,51,55;,
 4;52,56,57,53;,
 4;58,54,55,59;,
 4;56,60,61,57;,
 4;62,58,59,63;,
 4;28,33,64,65;,
 4;66,67,68,69;,
 4;65,64,70,71;,
 4;67,72,73,68;,
 4;71,70,74,75;,
 4;72,76,77,73;,
 4;75,74,78,79;,
 4;76,80,81,77;,
 4;79,78,82,83;,
 4;80,84,85,81;,
 4;83,82,86,87;,
 4;84,88,89,85;,
 4;87,86,90,91;,
 4;88,92,93,89;,
 4;91,90,33,28;,
 4;92,66,69,93;,
 3;94,95,96;,
 3;97,96,95;,
 3;98,95,99;,
 3;100,99,95;,
 3;94,101,102;,
 3;100,103,101;,
 3;98,101,103;,
 3;97,102,101;,
 3;98,104,105;,
 3;97,106,107;,
 3;97,108,104;,
 3;98,107,109;,
 3;100,105,104;,
 3;94,107,106;,
 3;94,104,108;,
 3;100,109,107;;
 
 MeshMaterialList {
  2;
  58;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.269804;0.718431;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.696471;0.627451;0.216471;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  42;
  -0.224726;0.948154;-0.224726;,
  0.000000;-1.000000;-0.000000;,
  0.224726;0.948154;-0.224726;,
  0.224726;0.948154;0.224726;,
  -0.224726;0.948154;0.224726;,
  0.897755;0.440494;0.000000;,
  0.000000;0.440494;-0.897755;,
  0.000000;0.440494;0.897755;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.897755;0.440494;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  -0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;-0.707107;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;-0.000000;,
  -0.707107;0.000000;-0.707107;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.827805;0.561016;0.000000;,
  -0.827805;0.561016;0.000000;,
  0.000000;-0.561016;-0.827805;,
  0.827805;0.561016;0.000000;,
  0.827805;-0.561016;-0.000000;,
  0.000000;-0.561016;0.827805;,
  -0.827805;0.561016;0.000000;,
  -0.827805;-0.561016;0.000000;,
  0.000000;0.561016;-0.827805;,
  0.000000;0.561016;-0.827805;,
  0.000000;0.561016;0.827805;,
  0.000000;0.561016;0.827805;;
  58;
  4;4,3,2,0;,
  4;1,1,1,1;,
  4;5,2,3,5;,
  4;5,5,8,8;,
  4;6,9,9,6;,
  4;6,6,0,2;,
  4;7,3,4,7;,
  4;7,7,10,10;,
  4;11,12,12,11;,
  4;11,11,4,0;,
  4;13,14,14,13;,
  4;18,21,21,18;,
  4;14,15,15,14;,
  4;22,18,18,22;,
  4;15,16,16,15;,
  4;23,22,22,23;,
  4;16,17,17,16;,
  4;24,23,23,24;,
  4;17,18,18,17;,
  4;25,24,24,25;,
  4;18,19,19,18;,
  4;26,25,25,26;,
  4;19,20,20,19;,
  4;27,26,26,27;,
  4;20,13,13,20;,
  4;21,27,27,21;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  3;30,30,33;,
  3;34,34,34;,
  3;34,34,34;,
  3;30,33,30;,
  3;36,36,31;,
  3;36,31,36;,
  3;37,37,37;,
  3;37,37,37;,
  3;32,32,32;,
  3;35,35,35;,
  3;32,32,32;,
  3;35,35,35;,
  3;38,39,38;,
  3;40,40,41;,
  3;38,38,39;,
  3;40,41,40;;
 }
 MeshTextureCoords {
  110;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.342672;,
  0.000000;0.000000;,
  1.000000;0.342672;,
  0.000000;1.000000;,
  1.000000;0.342672;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.342672;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.342672;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.342672;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.342672;,
  1.000000;1.000000;,
  0.000000;0.342672;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  0.250000;1.000000;,
  0.750000;1.000000;,
  1.000000;0.500000;,
  0.750000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;;
 }
}