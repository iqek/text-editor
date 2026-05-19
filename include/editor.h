#ifndef EDITOR_H
#define EDITOR_H

/*
    CSE232 Term Project - Text Editor with Garbage Collection

    Bu dosya projenin ortak header dosyasıdır.
    Herkes bu dosyadaki sabitleri, struct'ı, global değişkenleri
    ve fonksiyon prototiplerini kullanacak.

    Kerem: main.c, globals.c, Makefile, entegrasyon
    Merve: buffer.c
    Ipek: fileio.c
    Ilber: ui.c / ncurses
    Irem: commands.c, gc.c
*/


// Constants

#define MAX_LINES 100
/* Kullanılacağı dosyalar: globals.c, buffer.c, fileio.c, commands.c, gc.c, ui.c
   Kullanacak kişiler: HERKES
   Açıklama: textbuffer array'inin maksimum satır sayısı. projede textbuffer[100] isteniyor. */

#define MAX_LEN 40
//HERKES KULLANACAK
//Açıklama: Her satırdaki statement array'inin boyutu. PDF'te char statement[40] isteniyor.

#define MAX_SCREEN_LINES 30
/* Kullanılacağı dosyalar: ui.c, buffer.c
   Kullanacak kişiler: Merve, Ilber
   Açıklama: print işleminde ekranda maksimum 30 satır gösterileceği varsayılıyor idk. */

#define NIL -1
/* Kullanılacağı dosyalar: globals.c, buffer.c, fileio.c, commands.c, gc.c, ui.c
   HERKES
   Açıklama: Linked list bağlantılarında "yok/null index" anlamına gelir. */


// Data Structure

typedef struct node {
    char statement[MAX_LEN];
    //Açıklama: Satırın metnini tutar. Maksimum MAX_LEN karakterlik alan.
    int next;
    int prev;
} Node;

//Açıklama: PDF'teki struct node yapısının typedef edilmiş hali.


// Global Variables BURADAKI HERSEY "globals.c"'de tanımlancak.

extern Node textbuffer[MAX_LINES];
/*Kullanılacağı dosyalar: buffer.c, fileio.c, ui.c, commands.c, gc.c, main.c
   HERKES
   Açıklama: Bütün text satırlarının tutulduğu ana buffer. Projenin merkezi array'i. */

extern int head;
extern int tail;

extern int freeIndex;
/*Kullanılacağı dosyalar: fileio.c, commands.c, gc.c, buffer.c
   Açıklama: PDF'teki "free" değişkeninin karşılığı.
   Yeni satırın textbuffer[] içinde yazılacağı ilk boş indexi gösterir.*/

extern char currentFilename[256];
/*Kullanılacağı dosyalar: fileio.c, main.c
   Açıklama: E komutuyla açılan son dosya adını tutar.
   S komutunda dosyayı tekrar aynı isimle kaydetmek için kullanılır. */

extern int operationCount;
/*Kullanılacağı dosyalar: commands.c, gc.c, buffer.c
   Kullanacak kişiler: Merve ve Irem probably
   Açıklama: Insert/delete işlem sayısını tutar.
   her 10 insert/delete işleminden sonra otomatik garbage collection çalışcak. */

//---------------------------------------------------------------------------------------------------------------------------

// buffer.c - Kişi 2 (burası merve i guess)

void initBuffer(void);
//Açıklama: textbuffer, head, tail, freeIndex ve operationCount başlangıç ayarlarını yapar. (burada chatgpt valla anlamadım)

int getBufferIndexByScreenLine(int screenLine);
//Açıklama: Ekrandaki satır numarasını gerçek textbuffer[] indexine çevirir.

int validLineCount(void);
//Açıklama: head'den başlayıp next linklerini takip ederek geçerli satır sayısını bulur.

void debugPrintBuffer(void);
// main.c veya test amaçlı kullancaz
// Açıklama: Debug için textbuffer'ın iç durumunu terminale yazdırır.

//----------------------------------------------------------------------------------------------------------------------------

//fileio.c - IPOS

int editFile(const char *filename);
/* PDF'teki edit(char *filename) fonksiyonunun karşılığı.
   Dosyayı açar, satırları textbuffer içine koyar, linkleri kurar.
*/

int saveFile(void);
/* PDF'teki save() fonksiyonunun karşılığı.
   head'den başlayıp next linklerini takip ederek geçerli satırları dosyaya yazar. */

//----------------------------------------------------------------------------------------------------------------------------

// ui.c - ilber

void initUI(void);
//Açıklama: ncurses ekranını başlatır. ör: initscr(), cbreak(), noecho(), keypad().

void closeUI(void);
//Açıklama: ncurses ekranını kapatır. Örneğin endwin().

void printEditor(void);
// PDF'teki print() fonksiyonunun karşılığıdır.head'den başlayıp next linklerini takip ederek text'i ekranda gösterir.

int cursorLine(void);
//PDF'teki cursorLine() fonksiyonudur.Cursor'ın ekranda bulunduğu satırdan gerçek textbuffer[] indexini döndürür.

int cursorChar(void);
//PDF'teki cursorChar() fonksiyonudur.Cursor'ın seçili satırdaki karakter pozisyonunu döndürür.


//---------------------------------------------------------------------------------------------------------------------------

// commands.c - irem

int insertLineAfter(int index, const char *line);
/* Açıklama: PDF'teki insert(int index) fonksiyonunun karşılığıdır.
   Seçili satırdan sonra yeni satır ekler ve linkleri günceller. */

int deleteLine(int index);
/* PDF'teki delete(int index) fonksiyonunun karşılığıdır.
   Satırı fiziksel olarak silmez, linked list bağlantısından çıkarır. */

int replaceChar(int lineIndex, int charIndex, char newChar);
/* PDF'teki replace(int index) fonksiyonunun daha açık halidir.
   Seçili satırdaki seçili karakteri değiştirir.*/


// gc.c (GARBAGE COLLECTION) - yine irem

int garbageCollection(void);
/* PDF'teki garbageCollection() fonksiyonudur.
   Linklerden erişilebilen geçerli satırları başa sıkıştırır,kullanılmayan alanları textbuffer'ın sonuna bırakır.*/

void checkAutomaticGC(void);
//Açıklama: freeIndex dolduysa veya operationCount 10 olduysa otomatik garbage collection çalıştırır.

#endif
