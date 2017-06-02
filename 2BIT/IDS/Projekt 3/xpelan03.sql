DROP TABLE "Cestujici" CASCADE CONSTRAINTS;
CREATE TABLE "Cestujici"(
  "rodneCislo" NUMBER NOT NULL,
  "jmeno" VARCHAR2(30) NOT NULL,
  "pocetZavazadel" NUMBER NOT NULL
);

DROP TABLE "Gate" CASCADE CONSTRAINTS;
CREATE TABLE "Gate"(
  "cisloGate" NUMBER NOT NULL,
  "datumStavby" DATE NOT NULL,
  "terminal" NUMBER NOT NULL
);

DROP TABLE "Let" CASCADE CONSTRAINTS;
CREATE TABLE "Let"(
"cisloLetu" NUMBER NOT NULL,
"destinace" VARCHAR2(30) NOT NULL,
"casOdletu" DATE NOT NULL,
"ocekavanaDobaLetu" NUMBER NOT NULL,
"gate" NUMBER NOT NULL,
"letadlo" NUMBER NOT NULL 
);

DROP TABLE "Letadlo" CASCADE CONSTRAINTS;
CREATE TABLE "Letadlo"(
"cisloLetadla" NUMBER NOT NULL,
"datumPosledniRevize" DATE NULL,
"datumVyroby" DATE NOT NULL,
"pocetClenuPosadky" NUMBER NULL,
"vyrobce" VARCHAR2(30) NOT NULL,
"typLetadla" NUMBER NOT NULL 
);

DROP TABLE "PalubniVstupenka" CASCADE CONSTRAINTS;
CREATE TABLE "PalubniVstupenka"(
"cisloVstupenky" NUMBER NOT NULL,
"mistoVLetadle" VARCHAR2(30) NOT NULL,
"trida" VARCHAR2(30) NOT NULL,
"cestujici" NUMBER NOT NULL,
"let" NUMBER NOT NULL 
);

DROP TABLE "Terminal" CASCADE CONSTRAINTS;
CREATE TABLE "Terminal"(
"cisloTerminalu" NUMBER NOT NULL,
"kapacita" NUMBER NOT NULL,
"pocetParkovacichMist" NUMBER NOT NULL
);

DROP TABLE "TypLetadla" CASCADE CONSTRAINTS;
CREATE TABLE "TypLetadla"(
"cisloTypu" NUMBER NOT NULL,
"pocetMist" NUMBER NOT NULL,
"typRozmisteniMist" NUMBER NOT NULL
);

DROP TABLE "TypLetadla_Gate" CASCADE CONSTRAINTS;
CREATE TABLE "TypLetadla_Gate"(
"TypGateID" NUMBER NOT NULL,
"povolenyTyp" NUMBER NOT NULL,
"povolenyGate" NUMBER NOT NULL 
);

ALTER TABLE "Cestujici" ADD CONSTRAINT "PK_Cestujici" PRIMARY KEY ("rodneCislo");
ALTER TABLE "Gate" ADD CONSTRAINT "PK_Gate" PRIMARY KEY ("cisloGate");
ALTER TABLE "Let" ADD CONSTRAINT "PK_Let" PRIMARY KEY ("cisloLetu");
ALTER TABLE "Letadlo" ADD CONSTRAINT "PK_Letadlo" PRIMARY KEY ("cisloLetadla");
ALTER TABLE "PalubniVstupenka" ADD CONSTRAINT "PK_PalubniVstupenka" PRIMARY KEY ("cisloVstupenky");
ALTER TABLE "Terminal" ADD CONSTRAINT "PK_Terminal" PRIMARY KEY ("cisloTerminalu");
ALTER TABLE "TypLetadla" ADD CONSTRAINT "PK_TypLetadla" PRIMARY KEY ("cisloTypu");
ALTER TABLE "TypLetadla_Gate" ADD CONSTRAINT "PK_TypLetadla_Gate" PRIMARY KEY ("TypGateID");

ALTER TABLE "Gate" ADD CONSTRAINT "FK_terminal" FOREIGN KEY ("terminal") REFERENCES "Terminal" ("cisloTerminalu");
ALTER TABLE "Let" ADD CONSTRAINT "FK_gate" FOREIGN KEY ("gate") REFERENCES "Gate" ("cisloGate");
ALTER TABLE "Let" ADD CONSTRAINT "FK_letadlo" FOREIGN KEY ("letadlo") REFERENCES "Letadlo" ("cisloLetadla");
ALTER TABLE "Letadlo" ADD CONSTRAINT "FK_typLetadla" FOREIGN KEY ("typLetadla") REFERENCES "TypLetadla" ("cisloTypu");
ALTER TABLE "PalubniVstupenka" ADD CONSTRAINT "FK_cestujici" FOREIGN KEY ("cestujici") REFERENCES "Cestujici" ("rodneCislo");
ALTER TABLE "PalubniVstupenka" ADD CONSTRAINT "FK_let" FOREIGN KEY ("let") REFERENCES "Let" ("cisloLetu");
ALTER TABLE "TypLetadla_Gate" ADD CONSTRAINT "FK_povolenyGate" FOREIGN KEY ("povolenyGate") REFERENCES "Gate" ("cisloGate");
ALTER TABLE "TypLetadla_Gate" ADD CONSTRAINT "FK_povolenyTyp" FOREIGN KEY ("povolenyTyp") REFERENCES "TypLetadla" ("cisloTypu");

--Kontrola spravneho formatu rodneho cisla
CREATE OR REPLACE TRIGGER "TR_rodneCislo" BEFORE INSERT OR UPDATE OF "rodneCislo" ON "Cestujici"
FOR EACH ROW
DECLARE
  rc "Cestujici"."rodneCislo"%TYPE;
  den INTEGER;
  mesic INTEGER;
  rok INTEGER;
  koncovka INTEGER;
  
BEGIN
rc := :NEW."rodneCislo";            
IF(LENGTH(rc) >= 11 or LENGTH(rc) <= 8 ) THEN
   Raise_Application_Error (-20203, 'Chybna delka rodneho cisla');
END IF;

IF(LENGTH(rc) = 9) THEN
  den   := MOD( (rc / 1000), 100);
  mesic := MOD( (rc / 100000), 100);    
  rok   := rc / 10000000;            
  koncovka := MOD( rc, 1000);
END IF;

IF(LENGTH(rc) = 10) THEN
  den   := MOD( (rc / 10000), 100);
  mesic := MOD( (rc / 1000000), 100);   
  rok   := rc / 100000000;            
  koncovka := MOD( rc, 10000);
END IF;

IF (LENGTH(rc) = 9 and koncovka = 0) THEN
    Raise_Application_Error (-20203, 'Chybna koncovka rodneho cisla');
END IF;

IF (mesic > 50) THEN
  mesic := mesic - 50;
END IF;

IF (mesic > 20) THEN
  mesic := mesic - 20;
END IF;

IF (den > 40) THEN
  den := den - 40;
END IF;

IF (mesic < 1 or mesic > 12) THEN
  Raise_Application_Error (-20203, 'Chybny mesic rodneho cisla');
END IF;

IF (rok < 0 or rok > 99) THEN
  Raise_Application_Error (-20203, 'Chybny rok rodneho cisla');
END IF;

IF (MOD(mesic,2) = 1 and mesic < 8) THEN
  IF (den > 31) THEN
    Raise_Application_Error (-20203, 'Chybny den rodneho cisla');
  END IF;
ELSIF (MOD(mesic,2) = 0 and mesic < 8) THEN
  IF (mesic = 2) THEN
    IF (den > 29) THEN
      Raise_Application_Error (-20203, 'Chybny den rodneho cisla');
    END IF;
  ELSIF (den > 30) THEN
    Raise_Application_Error (-20203, 'Chybny den rodneho cisla');
  END IF;
END IF;

IF (MOD(mesic,2) = 0 and mesic > 7) THEN
  IF (den > 31) THEN
    Raise_Application_Error (-20203, 'Chybny den rodneho cisla');
  END IF;
ELSIF (MOD(mesic,2) = 1 and mesic > 7) THEN
  IF (den > 30) THEN
    Raise_Application_Error (-20203, 'Chybny den rodneho cisla');
  END IF;
END IF;

IF (LENGTH(rc) = 10 and MOD(rc, 11) <> 0) THEN
  Raise_Application_Error (-20203, 'Chybny format rodneho cisla');
END IF;

END "TR_rodneCislo";
/

INSERT INTO "Cestujici" ("rodneCislo", "jmeno", "pocetZavazadel") VALUES ('470922956', 'Alois Dočekal', '2');
INSERT INTO "Cestujici" ("rodneCislo", "jmeno", "pocetZavazadel") VALUES ('9262269852', 'Jiří Rosický', '1');
INSERT INTO "Cestujici" ("rodneCislo", "jmeno", "pocetZavazadel") VALUES ('6005139426', 'František Straka', '4');
INSERT INTO "Cestujici" ("rodneCislo", "jmeno", "pocetZavazadel") VALUES ('7555059633', 'Jaromír Jágr', '1');
INSERT INTO "Cestujici" ("rodneCislo", "jmeno", "pocetZavazadel") VALUES ('7807236415', 'František Doležal', '3');
INSERT INTO "Terminal" ("cisloTerminalu", "kapacita", "pocetParkovacichMist") VALUES ('1', '500', '100');
INSERT INTO "Terminal" ("cisloTerminalu", "kapacita", "pocetParkovacichMist") VALUES ('2', '350', '80');
INSERT INTO "Terminal" ("cisloTerminalu", "kapacita", "pocetParkovacichMist") VALUES ('3', '200', '50');
INSERT INTO "Terminal" ("cisloTerminalu", "kapacita", "pocetParkovacichMist") VALUES ('4', '400', '90');
INSERT INTO "TypLetadla" ("cisloTypu", "pocetMist", "typRozmisteniMist") VALUES ('1', '200', '5');
INSERT INTO "TypLetadla" ("cisloTypu", "pocetMist", "typRozmisteniMist") VALUES ('2', '50', '10');
INSERT INTO "TypLetadla" ("cisloTypu", "pocetMist", "typRozmisteniMist") VALUES ('3', '350', '2');
INSERT INTO "Gate" ("cisloGate", "datumStavby", "terminal") VALUES ('1', TO_DATE('2013-10-02 15:26:27', 'YYYY-MM-DD HH24:MI:SS'), '1');
INSERT INTO "Gate" ("cisloGate", "datumStavby", "terminal") VALUES ('2', TO_DATE('2013-07-30 15:28:51', 'YYYY-MM-DD HH24:MI:SS'), '2');
INSERT INTO "Gate" ("cisloGate", "datumStavby", "terminal") VALUES ('3', TO_DATE('2013-07-28 15:28:51', 'YYYY-MM-DD HH24:MI:SS'), '4');
INSERT INTO "Gate" ("cisloGate", "datumStavby", "terminal") VALUES ('4', TO_DATE('2013-04-30 15:28:51', 'YYYY-MM-DD HH24:MI:SS'), '3');
INSERT INTO "Gate" ("cisloGate", "datumStavby", "terminal") VALUES ('5', TO_DATE('2013-01-30 15:28:51', 'YYYY-MM-DD HH24:MI:SS'), '2');
INSERT INTO "Letadlo" ("cisloLetadla", "datumPosledniRevize", "datumVyroby", "pocetClenuPosadky", "vyrobce", "typLetadla") VALUES ('1', TO_DATE('2015-01-27 15:24:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2013-07-02 00:00:00', 'YYYY-MM-DD HH24:MI:SS'), '6', 'Boeing', '3');
INSERT INTO "Letadlo" ("cisloLetadla", "datumPosledniRevize", "datumVyroby", "pocetClenuPosadky", "vyrobce", "typLetadla") VALUES ('2', TO_DATE('2014-10-08 15:25:10', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2011-12-01 00:00:00', 'YYYY-MM-DD HH24:MI:SS'), '3', 'Cesna', '2');
INSERT INTO "Letadlo" ("cisloLetadla", "datumPosledniRevize", "datumVyroby", "pocetClenuPosadky", "vyrobce", "typLetadla") VALUES ('3', TO_DATE('2014-10-08 15:25:10', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2011-12-01 00:00:00', 'YYYY-MM-DD HH24:MI:SS'), '5', 'Boeing', '3');
INSERT INTO "Let" ("cisloLetu", "destinace", "casOdletu", "ocekavanaDobaLetu", "gate", "letadlo") VALUES ('1', 'Berlín', TO_DATE('2015-07-02 15:30:35', 'YYYY-MM-DD HH24:MI:SS'), '220', '1', '1');
INSERT INTO "Let" ("cisloLetu", "destinace", "casOdletu", "ocekavanaDobaLetu", "gate", "letadlo") VALUES ('2', 'Londýn',  TO_DATE('2015-12-04 15:31:09', 'YYYY-MM-DD HH24:MI:SS'), '80', '2', '2');
INSERT INTO "Let" ("cisloLetu", "destinace", "casOdletu", "ocekavanaDobaLetu", "gate", "letadlo") VALUES ('3', 'New York', TO_DATE('2015-12-04 15:31:09', 'YYYY-MM-DD HH24:MI:SS'), '80', '3', '3');
INSERT INTO "Let" ("cisloLetu", "destinace", "casOdletu", "ocekavanaDobaLetu", "gate", "letadlo") VALUES ('5', 'Praha', TO_DATE('2015-12-04 15:31:09', 'YYYY-MM-DD HH24:MI:SS'), '80', '4', '1');
INSERT INTO "Let" ("cisloLetu", "destinace", "casOdletu", "ocekavanaDobaLetu", "gate", "letadlo") VALUES ('4', 'Londýn', TO_DATE('2015-12-04 15:31:09', 'YYYY-MM-DD HH24:MI:SS'), '80', '5', '2');
INSERT INTO "PalubniVstupenka" ("cisloVstupenky", "mistoVLetadle", "trida", "cestujici", "let") VALUES ('1', 'U okénka', '1', '470922956', '1');
INSERT INTO "PalubniVstupenka" ("cisloVstupenky", "mistoVLetadle", "trida", "cestujici", "let") VALUES ('2', 'U uličky', '3', '9262269852', '2');
INSERT INTO "PalubniVstupenka" ("cisloVstupenky", "mistoVLetadle", "trida", "cestujici", "let") VALUES ('3', 'U uličky', '3', '6005139426', '3');
INSERT INTO "PalubniVstupenka" ("cisloVstupenky", "mistoVLetadle", "trida", "cestujici", "let") VALUES ('4', 'U uličky', '3', '7555059633', '5');
INSERT INTO "PalubniVstupenka" ("cisloVstupenky", "mistoVLetadle", "trida", "cestujici", "let") VALUES ('5', 'U uličky', '3', '7807236415', '4');
INSERT INTO "TypLetadla_Gate" ("TypGateID", "povolenyTyp", "povolenyGate") VALUES ('1', '3', '2');
INSERT INTO "TypLetadla_Gate" ("TypGateID", "povolenyTyp", "povolenyGate") VALUES ('2', '1', '2');
INSERT INTO "TypLetadla_Gate" ("TypGateID", "povolenyTyp", "povolenyGate") VALUES ('3', '2', '1');

--Spojeni dvou tabulek, vypis vsech letu a konkretni gate a terminal, ze kterych odleta 
SELECT L."cisloLetu", L."destinace", L."gate", G."terminal", L."casOdletu" FROM "Let" L, "Gate" G WHERE L."gate" = G."cisloGate";

--Spojeni dvou tabulek, vypis vsech letadel a typu konkretniho letadla
SELECT L."cisloLetadla", L."datumVyroby", L."datumPosledniRevize", L."pocetClenuPosadky", L."vyrobce", T."cisloTypu", T."pocetMist", T."typRozmisteniMist" FROM "Letadlo" L, "TypLetadla" T WHERE L."typLetadla" = T."cisloTypu";

--Spojeni tri tabulek, vypis informaci o cestujicim, palubni vstupence a letu
SELECT Z."rodneCislo", Z."jmeno", Z."pocetZavazadel", P."trida", P."mistoVLetadle", L."casOdletu", L."destinace", L."ocekavanaDobaLetu"  FROM "PalubniVstupenka" P, "Cestujici" Z, "Let" L WHERE P."cestujici" = Z."rodneCislo" and P."let" = L."cisloLetu";

--Dotaz s klauzuli GROUP BY a agregacni funkci count, vypise vyrobce letadel a pocet letadel od konkretniho vyrobce 
SELECT L."vyrobce", count(L."vyrobce") as "pocet letadel" FROM "Letadlo" L GROUP BY L."vyrobce";

--Dotaz s klauzuli GROUP BY a agregacni funkci count, vypise cislo letadla a kolik ma prirazenych letu
SELECT L."cisloLetadla", count(F."letadlo") as "pocet letu" FROM "Letadlo" L, "Let" F WHERE L."cisloLetadla" = F."letadlo" GROUP BY F."letadlo", L."cisloLetadla";

--Dotaz s prediktatem EXISTS, vybere cestujici, kteri leti do Londyna
SELECT Z."rodneCislo", Z."jmeno", Z."pocetZavazadel" FROM "Cestujici" Z WHERE EXISTS(SELECT L."cisloLetu" FROM "Let" L, "PalubniVstupenka" P WHERE Z."rodneCislo" = P."cestujici" and P."let" = L."cisloLetu" and L."destinace" = 'Londýn'); 

--Dotaz s prediktatem IN, vypise lety, ktere odletaji z terminalu, jejichz kapacita je vetsi nebo rovna 400
SELECT L."cisloLetu", L."destinace", L."casOdletu", L."gate" FROM "Let" L WHERE L."gate" IN (SELECT G."cisloGate" FROM "Gate" G, "Terminal" T WHERE G."terminal" = T."cisloTerminalu" and T."kapacita" >= 400);
