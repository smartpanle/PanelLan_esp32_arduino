
#include "FS.h"
#include "SD_MMC.h"
#include "SD.h"
#include "PanelLan.h"

// BOARD_SC01, BOARD_SC01_PLUS, BOARD_SC02, BOARD_SC05, BOARD_KC01, BOARD_BC02
PanelLan tft(BOARD_SC01_PLUS);

void listDir(fs::FS& fs, const char* dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    tft.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    tft.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS& fs, const char* path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
    tft.println("Dir created");
  } else {
    Serial.println("mkdir failed");
    tft.println("mkdir failed");
  }
}

void removeDir(fs::FS& fs, const char* path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
    tft.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
    tft.println("rmdir failed");
  }
}

void readFile(fs::FS& fs, const char* path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    tft.println("Failed to open file for readin");
    return;
  }

  Serial.print("Read from file: ");
  tft.print("Read from file:");
  while (file.available()) {
    Serial.write(file.read());
    tft.print(file.read());
  }
}

void writeFile(fs::FS& fs, const char* path, const char* message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    tft.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
    tft.println("File written");
  } else {
    Serial.println("Write failed");
    tft.println("Write failed");
  }
}

void appendFile(fs::FS& fs, const char* path, const char* message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    tft.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
    tft.println("Message appended");
  } else {
    Serial.println("Append failed");
    tft.println("Append failed");
  }
}

void renameFile(fs::FS& fs, const char* path1, const char* path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
    tft.println("File renamed");
  } else {
    Serial.println("Rename failed");
    tft.println("Rename failed");
  }
}

void deleteFile(fs::FS& fs, const char* path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
    tft.println("File deleted");
  } else {
    Serial.println("Delete failed");
    tft.println("Delete failed");
  }
}

void testFileIO(fs::FS& fs, const char* path) {
  File           file = fs.open(path);
  static uint8_t buf[512];
  size_t         len   = 0;
  uint32_t       start = millis();
  uint32_t       end   = start;
  if (file) {
    len         = file.size();
    size_t flen = len;
    start       = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) { file.write(buf, 512); }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

void setup() {
  Serial.begin(115200);
  tft.begin();

  if ((tft.pins.sd.clk == -1) || (tft.pins.sd.cmd == -1) || (tft.pins.sd.d0 == -1) || (tft.pins.sd.d3 == -1)) {
    Serial.println("The current board does not support SD");
    tft.println("The current board does not support SD");
    return;
  }

  SPI.begin(tft.pins.sd.clk, tft.pins.sd.d0, tft.pins.sd.cmd, tft.pins.sd.d3);
  if (!SD.begin(tft.pins.sd.d3, SPI)) {
    Serial.println("Card Mount Failed");
    tft.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    tft.println("No SD card attached");
    return;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  tft.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  removeDir(SD, "/mydir");
  listDir(SD, "/", 2);
  tft.println("\n1.WriteFile hello.txt write Hello");
  writeFile(SD, "/hello.txt", "Hello ");
  tft.println("\n2.AppendFile hello.txt append World");
  appendFile(SD, "hello.txt", "World!\n");
  tft.println("\n3.ReadFile hello.txt");
  readFile(SD, "/hello.txt");
  tft.println("\n4.DeleteFile foo.txt");
  deleteFile(SD, "/foo.txt");
  tft.println("\n5.RenameFile hello.txt to foo.txt");
  renameFile(SD, "/hello.txt", "/foo.txt");
  tft.println("\n6.ReadFile foo.txt");
  readFile(SD, "/foo.txt");
  tft.println("\n7.TestFileIO test.txt");
  testFileIO(SD, "/test.txt");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  tft.printf("\nTotal space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  tft.printf("\nUsed space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop(void) {
    
}
