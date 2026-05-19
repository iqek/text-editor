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
```
---

# Teknik Tasarım ve Ekip Notları

Bu bölüm, projeyi geliştiren ekip üyelerinin aynı mantıkla ilerlemesi için hazırlanmıştır.  
Kod yazarken özellikle `textbuffer[]`, `head`, `tail`, `freeIndex`, linked list bağlantıları ve garbage collection mantığı aynı şekilde kullanılmalıdır.

---

## Ana Global Değişkenler

Projede text editor belleği aşağıdaki global değişkenler üzerinden yönetilecektir:

```c
Node textbuffer[100];

int head;
int tail;
int freeIndex;
```

| Değişken / Alan | Anlamı |
|---|---|
| `textbuffer[100]` | En fazla 100 satırlık text buffer |
| `statement[40]` | Her satır için maksimum 40 karakterlik metin alanı |
| `next` | Sonraki geçerli satırın `textbuffer[]` indexi |
| `prev` | Önceki geçerli satırın `textbuffer[]` indexi |
| `head` | İlk geçerli satırın `textbuffer[]` indexi |
| `tail` | Son geçerli satırın `textbuffer[]` indexi |
| `freeIndex` | Yeni satırın yazılacağı ilk boş index |

> Not: Proje PDF’inde değişken adı `free` olarak verilmiştir.  
> Ancak C dilinde `free()` fonksiyonu bulunduğu için kodda `freeIndex` adı kullanılacaktır.  
> `freeIndex`, PDF’teki `free` değişkeninin karşılığıdır.

---

## Desteklenecek Komutlar

Editor aşağıdaki komutları destekleyecektir:

| Komut | Anlamı |
|---|---|
| `E filename` | Dosyayı açar ve text buffer’a yükler |
| `P` | Text’i ekranda gösterir |
| `I` | Seçili satırdan sonra yeni satır ekler |
| `D` | Seçili satırı siler |
| `R` | Seçili karakteri değiştirir |
| `S` | Dosyayı kaydeder |
| `G` | Garbage collection çalıştırır |
| `Q` | Editörden çıkar |

---

## Hedeflenen Klasör Yapısı

Proje klasör yapısı şu şekilde tutulacaktır:

```txt
text-editor/
├── include/
│   └── editor.h
├── src/
│   ├── main.c
│   ├── globals.c
│   ├── buffer.c
│   ├── fileio.c
│   ├── ui.c
│   ├── commands.c
│   └── gc.c
├── tests/
│   └── sample.txt
├── docs/
├── Makefile
├── README.md
└── .gitignore
```

---

# Dosyaların Anlamı ve Sorumluluklar

## `include/editor.h`

Bu dosya projenin ortak header dosyasıdır.

Burada şunlar bulunur:

- Sabitler
- `Node` struct tanımı
- Global değişkenlerin `extern` tanımları
- Fonksiyon prototipleri

Önemli:

```c
int saveFile(void);
```

gibi satırlar sadece fonksiyonun var olduğunu söyler.  
Fonksiyonun gerçek içi `.c` dosyalarında yazılır.

Örneğin:

```c
// include/editor.h
int saveFile(void);
```

```c
// src/fileio.c
int saveFile(void) {
    // gerçek kod burada
}
```

---

## `src/globals.c`

Bu dosyada global değişkenlerin gerçek tanımları yapılır.

Sorumlu kişi: **Kerem**

Örnek:

```c
Node textbuffer[MAX_LINES];

int head = NIL;
int tail = NIL;
int freeIndex = 0;
```

Bu dosya olmadan `editor.h` içindeki `extern` tanımlar gerçek belleğe karşılık gelmez.

---

## `src/main.c`

Ana program döngüsü burada olacaktır.

Sorumlu kişi: **Kerem**

Görevleri:

- Kullanıcıdan komut almak
- Komuta göre ilgili fonksiyonu çağırmak
- Programın genel akışını yönetmek
- Diğer ekip üyelerinin yazdığı modülleri bir araya getirmek

Örnek komutlar:

```txt
E tests/sample.txt
P
S
Q
```

---

## `src/buffer.c`

Text buffer ile ilgili yardımcı fonksiyonlar burada olacaktır.

Sorumlu kişi: **Merve**

Merve’nin sorumlu olduğu fonksiyonlar:

```c
void initBuffer(void);
int getBufferIndexByScreenLine(int screenLine);
int validLineCount(void);
void debugPrintBuffer(void);
```

Bu dosyanın amacı:

- Buffer’ı başlatmak
- `head`, `tail`, `freeIndex` durumunu yönetmek
- Ekrandaki satır numarası ile gerçek `textbuffer[]` indexi arasında bağlantı kurmak
- Debug çıktısı üretmek

Önemli nokta:

Ekrandaki 3. satır her zaman `textbuffer[3]` değildir.  
Çünkü insert/delete işlemlerinden sonra array sırası ile ekrandaki görünüm farklı olabilir.

Bu yüzden gerçek sıralama her zaman `head`’den başlayıp `next` linkleri takip edilerek bulunmalıdır.

---

## `src/fileio.c`

Dosya açma ve kaydetme işlemleri burada olacaktır.

Sorumlu kişi: **İpek**

İpek’in sorumlu olduğu fonksiyonlar:

```c
int editFile(const char *filename);
int saveFile(void);
```

Bu dosyanın amacı:

- `.txt` dosyasını açmak
- Satır satır okuyup `textbuffer[]` içine koymak
- `prev` ve `next` bağlantılarını kurmak
- Dosyayı tekrar kaydetmek

Önemli:

Save işlemi sırasında array baştan sona yazılmamalıdır.

Yanlış mantık:

```c
for (int i = 0; i < freeIndex; i++) {
    fprintf(file, "%s\n", textbuffer[i].statement);
}
```

Çünkü silinmiş ama bellekte duran satırlar da dosyaya yazılabilir.

Doğru mantık:

```c
int current = head;

while (current != NIL) {
    fprintf(file, "%s\n", textbuffer[current].statement);
    current = textbuffer[current].next;
}
```

Yani sadece linked list üzerinden erişilebilen geçerli satırlar kaydedilmelidir.

---

## `src/ui.c`

Ekran ve cursor işlemleri burada olacaktır.

Sorumlu kişi: **İlber**

İlber’in sorumlu olduğu fonksiyonlar:

```c
void initUI(void);
void closeUI(void);
void printEditor(void);
int cursorLine(void);
int cursorChar(void);
```

Bu dosyanın amacı:

- `ncurses` başlatmak
- Text’i ekrana yazdırmak
- Cursor’u yukarı/aşağı/sağa/sola hareket ettirmek
- Seçili satırı bulmak
- Seçili karakter pozisyonunu bulmak

Kullanılacak temel `ncurses` fonksiyonları:

```c
initscr();
cbreak();
noecho();
keypad(stdscr, TRUE);
getch();
move(y, x);
mvprintw(y, x, "...", ...);
clear();
refresh();
endwin();
```

Önemli:

`cursorLine()` fonksiyonu ekrandaki satırı gerçek `textbuffer[]` indexine çevirmelidir.  
Bunun için Merve’nin yazacağı `getBufferIndexByScreenLine()` fonksiyonu kullanılabilir.

---

## `src/commands.c`

Editor komutlarının asıl düzenleme işlemleri burada olacaktır.

Sorumlu kişi: **İrem**

İrem’in sorumlu olduğu fonksiyonlar:

```c
int insertLineAfter(int index, const char *line);
int deleteLine(int index);
int replaceChar(int lineIndex, int charIndex, char newChar);
```

Bu dosyanın amacı:

- Satır eklemek
- Satır silmek
- Karakter değiştirmek
- Linkleri doğru güncellemek

Delete işlemi dikkatli yapılmalıdır.

Önemli:

Delete işlemi satırı fiziksel olarak array’den silmez.  
Sadece linked list bağlantısından çıkarır.

Örneğin `bbb` silinirse:

```txt
aaa
bbb
ccc
```

görünüm şu olur:

```txt
aaa
ccc
```

Ama `bbb` hâlâ `textbuffer[]` içinde durabilir.  
Garbage collection yapılana kadar sadece erişilemez hale gelir.

---

## `src/gc.c`

Garbage collection işlemi burada olacaktır.

Sorumlu kişi: **İrem**

İrem’in sorumlu olduğu fonksiyonlar:

```c
int garbageCollection(void);
void checkAutomaticGC(void);
```

Bu dosyanın amacı:

- Silinmiş ama bellekte duran satırları temizlemek
- Geçerli satırları array’in başına sıkıştırmak
- `head`, `tail`, `freeIndex` değerlerini yeniden ayarlamak

Garbage collection şu durumlarda çalışmalıdır:

1. Kullanıcı `G` komutu verdiğinde
2. `freeIndex` dolduğunda
3. Her 10 insert/delete işleminden sonra

Garbage collection sonrası geçerli satırlar `textbuffer[0]`, `textbuffer[1]`, `textbuffer[2]` şeklinde başa taşınmalıdır.

---

# Ekip İçin Önemli Kurallar

Kod yazarken aşağıdaki kurallara uyulmalıdır:

1. `editor.h` içindeki fonksiyon isimleri değiştirilmemelidir.
2. Yeni global değişken gerekiyorsa önce ekipte konuşulmalıdır.
3. `textbuffer[]`, `head`, `tail`, `freeIndex` herkes tarafından aynı mantıkla kullanılmalıdır.
4. Silinen satırlar hemen array’den kaldırılmamalıdır.
5. Save işlemi sadece geçerli linked list sırasına göre yapılmalıdır.
6. UI kısmı tamamlanmadan önce fonksiyonlar terminal/debug çıktısıyla test edilebilir.
7. Kod yazarken önce küçük test yapılmalı, sonra GitHub’a gönderilmelidir.
8. `main` branch doğrudan bozulmamalıdır.
9. Herkes mümkünse kendi branch’i üzerinde çalışmalıdır.
10. Merge sonrası mutlaka `make clean`, `make` ve temel çalıştırma testi yapılmalıdır.

---

# Öncelikli Yapılacaklar

## 1. Kerem

Sorumlu olduğu alan:

- Proje iskeleti
- `main.c`
- `globals.c`
- `Makefile`
- README
- Entegrasyon

Yapılacaklar:

- [ ] `editor.h` son halini kontrol et
- [ ] `Makefile` çalışıyor mu kontrol et
- [ ] `main.c` içinde temel komut döngüsünü kur
- [ ] Repo yapısını düzenle
- [ ] Herkesin branch’lerini takip et
- [ ] Merge sonrası projeyi derle ve test et

---

## 2. Merve

Sorumlu olduğu alan:

- Text buffer
- Linked list yardımcı fonksiyonları
- Debug fonksiyonları

Yapılacaklar:

- [ ] `initBuffer()` fonksiyonunu tamamla
- [ ] `getBufferIndexByScreenLine()` fonksiyonunu yaz
- [ ] `validLineCount()` fonksiyonunu yaz
- [ ] `debugPrintBuffer()` fonksiyonunu test et
- [ ] `head`, `tail`, `freeIndex` değerlerinin doğru güncellendiğini kontrol et

---

## 3. İpek

Sorumlu olduğu alan:

- Dosya açma
- Dosya kaydetme
- Text’i buffer’a yükleme

Yapılacaklar:

- [ ] `editFile()` fonksiyonunu tamamla
- [ ] Dosyadan satır okuma işlemini yap
- [ ] Satırları `textbuffer[]` içine yerleştir
- [ ] `prev` ve `next` bağlantılarını kur
- [ ] `saveFile()` fonksiyonunu yaz
- [ ] Save sırasında sadece geçerli satırları yazdığını test et

---

## 4. İlber

Sorumlu olduğu alan:

- `ncurses`
- Ekran yenileme
- Cursor hareketleri
- Satır ve karakter seçimi

Yapılacaklar:

- [ ] `initUI()` fonksiyonunu yaz
- [ ] `closeUI()` fonksiyonunu yaz
- [ ] `printEditor()` fonksiyonunu ncurses ile yaz
- [ ] Cursor hareketlerini ekle
- [ ] `cursorLine()` fonksiyonunu tamamla
- [ ] `cursorChar()` fonksiyonunu tamamla
- [ ] Ok tuşları ve Enter seçimini test et

---

## 5. İrem

Sorumlu olduğu alan:

- Insert
- Delete
- Replace
- Garbage collection

Yapılacaklar:

- [ ] `insertLineAfter()` fonksiyonunu yaz
- [ ] `deleteLine()` fonksiyonunu yaz
- [ ] `replaceChar()` fonksiyonunu yaz
- [ ] `garbageCollection()` fonksiyonunu yaz
- [ ] `checkAutomaticGC()` fonksiyonunu yaz
- [ ] 10 işlemden sonra otomatik garbage collection çalışıyor mu test et

---

# Test Edilecek Durumlar

Minimum test listesi:

| No | Test | Durum |
|---|---|---|
| 1 | Boş dosya açılıyor mu? | ⬜ |
| 2 | Normal dosya açılıyor mu? | ⬜ |
| 3 | 1 satırlık dosya açılıyor mu? | ⬜ |
| 4 | 30 satırlık dosya ekranda düzgün görünüyor mu? | ⬜ |
| 5 | 100 satır sınırı çalışıyor mu? | ⬜ |
| 6 | Insert head sonrası çalışıyor mu? | ⬜ |
| 7 | Insert tail sonrası çalışıyor mu? | ⬜ |
| 8 | Delete head çalışıyor mu? | ⬜ |
| 9 | Delete tail çalışıyor mu? | ⬜ |
| 10 | Delete ortadan çalışıyor mu? | ⬜ |
| 11 | Replace doğru karakteri değiştiriyor mu? | ⬜ |
| 12 | Save sadece geçerli satırları yazıyor mu? | ⬜ |
| 13 | Garbage collection sonrası buffer sıkışıyor mu? | ⬜ |
| 14 | 10 işlemden sonra otomatik garbage collection çalışıyor mu? | ⬜ |
| 15 | Program `Q` ile düzgün kapanıyor mu? | ⬜ |

---

# Derleme ve Çalıştırma Kontrolü

Her önemli değişiklikten sonra şu komutlar çalıştırılmalıdır:

```bash
make clean
make
./editor
```

Eğer `ncurses` eksikse Ubuntu üzerinde şu komut kullanılabilir:

```bash
sudo apt update
sudo apt install libncurses5-dev libncursesw5-dev
```

---

# Önerilen Git Akışı

Herkes kendi branch’inde çalışmalıdır.

Örnek branch isimleri:

```txt
feature/buffer
feature/fileio
feature/ui-ncurses
feature/commands
feature/gc
```

Branch açmak için:

```bash
git checkout -b feature/buffer
```

Değişiklikleri kaydetmek için:

```bash
git add .
git commit -m "Implement buffer functions"
```

GitHub’a göndermek için:

```bash
git push origin feature/buffer
```

Sonrasında GitHub üzerinden Pull Request açılmalıdır.

---

# Şu Anki Genel Hedef

Öncelikli hedef, bütün dosyaların iskelet halinde çalışmasıdır.

İlk aşamada amaç:

- Projenin `make` ile derlenmesi
- `./editor` ile çalışması
- `E`, `P`, `S`, `Q` komutlarının temel seviyede denenebilmesi
- Diğer ekip üyelerinin kendi dosyalarını rahatça geliştirebilmesi

Sonrasında sırasıyla:

1. Buffer sistemi tamamlanacak.
2. File I/O tamamlanacak.
3. Insert/delete/replace işlemleri tamamlanacak.
4. Garbage collection tamamlanacak.
5. Ncurses UI bağlanacak.
6. Tüm sistem birlikte test edilecek.
