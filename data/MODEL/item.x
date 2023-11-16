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
 16;
 0.00000;76.81980;0.00000;,
 10.30735;50.38500;-10.30735;,
 0.00000;50.38500;-15.22080;,
 0.00000;76.81980;0.00000;,
 -10.30735;50.38500;-10.30735;,
 -10.30735;50.38500;10.30735;,
 0.00000;50.38500;15.22080;,
 10.30735;50.38500;10.30735;,
 0.00000;23.95020;0.00000;,
 0.00000;23.95020;0.00000;,
 15.22080;50.38500;0.00000;,
 10.30735;50.38500;-10.30735;,
 10.30735;50.38500;10.30735;,
 -15.22080;50.38500;0.00000;,
 -10.30735;50.38500;10.30735;,
 -10.30735;50.38500;-10.30735;;
 
 16;
 3;0,1,2;,
 3;3,2,4;,
 3;0,5,6;,
 3;3,6,7;,
 3;8,2,1;,
 3;9,4,2;,
 3;9,7,6;,
 3;8,6,5;,
 3;3,10,11;,
 3;0,12,10;,
 3;3,13,14;,
 3;0,15,13;,
 3;9,11,10;,
 3;8,10,12;,
 3;9,14,13;,
 3;8,13,15;;
 
 MeshMaterialList {
  1;
  16;
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
   0.800000;0.800000;0.800000;0.940000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  16;
  -0.707107;-0.000000;-0.707107;,
  0.000000;0.498983;-0.866612;,
  0.707107;-0.000000;-0.707107;,
  0.866612;0.498983;0.000000;,
  0.707107;-0.000000;0.707107;,
  0.000000;0.498983;0.866612;,
  0.314382;0.543050;-0.778627;,
  -0.707107;-0.000000;0.707107;,
  -0.866612;0.498983;0.000000;,
  0.314382;-0.543050;-0.778627;,
  0.314382;0.543050;0.778627;,
  0.000000;-0.498983;-0.866612;,
  0.314382;-0.543050;0.778627;,
  0.000000;-0.498983;0.866612;,
  0.866612;-0.498983;-0.000000;,
  -0.866612;-0.498983;0.000000;;
  16;
  3;6,2,1;,
  3;6,1,0;,
  3;10,7,5;,
  3;10,5,4;,
  3;9,11,2;,
  3;9,0,11;,
  3;12,4,13;,
  3;12,13,7;,
  3;6,3,2;,
  3;10,4,3;,
  3;8,8,7;,
  3;8,0,8;,
  3;9,2,14;,
  3;12,14,4;,
  3;15,7,15;,
  3;15,15,0;;
 }
 MeshTextureCoords {
  16;
  0.750000;0.000000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  0.250000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  0.750000;1.000000;,
  0.250000;1.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;;
 }
}
