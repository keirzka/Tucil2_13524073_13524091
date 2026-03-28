# Deskripsi Program
Program ini merupakan implementasi algoritma Divide and Conquer untuk melakukan voxelization objek 3D berbasis file .obj dengan memanfaatkan struktur data octree.

Input berupa file .obj yang berisi:

v x y z → vertex (titik 3D)  
f i j k → face (segitiga)

Program akan:
1. Membaca dan memvalidasi file .obj
2. Menghitung Axis-Aligned Bounding Box (AABB) dari objek
3. Membangun struktur octree secara rekursif
4. Melakukan subdivision (divide) hanya pada area yang mengandung objek
5. Melakukan pruning pada area kosong
6. Menghasilkan representasi voxel (kubus) dari objek
7. Menyimpan hasil dalam file .obj baru

Sebagai optimasi, program juga mengimplementasikan concurrency (multithreading) menggunakan std::async untuk mempercepat proses subdivisi octree.

# Requirement Program
Bahasa & Compiler  
C++ (minimal C++11, disarankan C++17)  
Compiler:  
GCC / G++ (MinGW / Linux / WSL)  
🔹 Library yang Digunakan  
Standard C++ Library:
```
<vector>
<iostream>
<fstream>
<chrono>
<future>
<algorithm>
<atomic>
<limits>
```
Graphics Library:
Raylib
🔹 Sistem Operasi  
Windows (MinGW / PowerShell / CMD)  
Linux / WSL (opsional)  

# Prasyarat Instalasi (Windows)
Agar 3D Viewer bisa dipakai, siapkan library Raylib:
1. Unduh raylib-5.0_win64_mingw-w64.zip (atau versi terbaru) dari GitHub Releases Raylib
2. Ekstrak isi file .zip ke direktori utama C:\
3. Ubah nama folder hasil ekstrak menjadi raylib
4. Jika menggunakan VS Code, tambahkan C:\raylib\include ke dalam Include Path pada konfigurasi C/C++.

# Cara Kompilasi Program
Jalankan perintah berikut di terminal pada folder src:

``` g++ -std=c++17 main.cpp parser.cpp geometry.cpp octree.cpp writer.cpp viewer.cpp -o voxelizer.exe -I C:/raylib/include -L C:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm```

# Cara Menjalankan Program
Ketik command untuk run program utama:
```./main```

Berikan masukan nama file objek yang akan dilakukan voxelization dengan format objek (.obj) dan maxDepth sesuai permintaan program

Notes : pastikan file objek berada di path yang benar (test/Objek)

# Identitas Author
1. Anggota 1  
   Nama : Keisha Rizka Syofyani  
   NIM : 13524073
   
2. Anggota 2  
   Nama : Vara Azzara Ramli Pulukadang  
   NIM : 13524091  
