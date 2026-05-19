# CSE232 Text Editor Project

Bu repository, CSE232 Systems Programming dersi term project'i için hazırlanmıştır.

Projenin amacı C dilinde, Linux üzerinde `gcc` ile derlenen, `ncurses` kullanan basit bir text editor geliştirmektir.

Editor; dosya açma, satır ekleme, satır silme, karakter değiştirme, ekrana yazdırma, dosyaya kaydetme ve garbage collection işlemlerini destekleyecektir.

---

## Proje Ekibi ve Görev Dağılımı

| Kişi | Görev Alanı | Dosyalar |
|---|---|---|
| Kerem | Proje iskeleti, main loop, Makefile, entegrasyon | `main.c`, `globals.c`, `Makefile`, `README.md` |
| Merve | Text buffer ve linked list yardımcı fonksiyonları | `buffer.c` |
| İpek | Dosya açma ve kaydetme işlemleri | `fileio.c` |
| İlber | ncurses arayüzü, ekran ve cursor işlemleri | `ui.c` |
| İrem | Insert, delete, replace ve garbage collection | `commands.c`, `gc.c` |

---

## Projenin Temel Mantığı

Editor, text dosyasındaki satırları normal bir string array gibi değil, array içinde linked list mantığıyla tutar.

Ana veri yapısı:

```c
typedef struct node {
    char statement[40];
    int next;
    int prev;
} Node;
