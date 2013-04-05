CREATE TABLE brew (brewSerial INTEGER PRIMARY KEY AUTOINCREMENT, startedDate DATE, initialFermentationDate DATE, bottledDate DATE, statusSerial NUMERIC NOT NULL DEFAULT 0, brewLabel TEXT);



CREATE TABLE "note" (noteSerial INTEGER PRIMARY KEY AUTOINCREMENT,  noteTypeSerial NUMERIC,  message TEXT,  createdDate DATE);



CREATE TABLE noteType (noteTypeSerial INTEGER PRIMARY KEY AUTOINCREMENT, noteTypeLabel TEXT);

INSERT INTO noteType (noteTypeSerial,noteTypeLabel) VALUES (1,'Note');
INSERT INTO noteType (noteTypeSerial,noteTypeLabel) VALUES (2,'Error');
INSERT INTO noteType (noteTypeSerial,noteTypeLabel) VALUES (3,'Idea');


CREATE TABLE review (reviewSerial INTEGER PRIMARY KEY AUTOINCREMENT, createdDate DATE, shortDescription TEXT, longDescription TEXT, rating NUMERIC);



CREATE TABLE sgReading (sgReadingSerial INTEGER PRIMARY KEY AUTOINCREMENT, sgValue NUMERIC);



CREATE TABLE status (statusSerial INTEGER PRIMARY KEY AUTOINCREMENT, statusLabel TEXT);

INSERT INTO status (statusSerial,statusLabel) VALUES (1,'Not Started');
INSERT INTO status (statusSerial,statusLabel) VALUES (2,'Initial Fermentation');
INSERT INTO status (statusSerial,statusLabel) VALUES (3,'Bottle Fermentation');
INSERT INTO status (statusSerial,statusLabel) VALUES (4,'Bottled/Ageing');
INSERT INTO status (statusSerial,statusLabel) VALUES (5,'Ready to Drink');
INSERT INTO status (statusSerial,statusLabel) VALUES (6,'Empty');


