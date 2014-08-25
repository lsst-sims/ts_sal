DROP TABLE IF EXISTS m1m3_Application_items;
CREATE TABLE m1m3_Application_items (
  num	int,
  item	char(32),
  type  char(32),
  size  int,
  units char(32),
  freq  float,
  range char(32),
  location char(32),
  comment char(128),
  PRIMARY KEY (num)
);
INSERT INTO m1m3_Application_items VALUES (1,"demand","float",6,"",0.054,"","","");
INSERT INTO m1m3_Application_items VALUES (2,"position","float",6,"",0.054,"","","");
INSERT INTO m1m3_Application_items VALUES (3,"error","float",6,"",0.054,"","","");
INSERT INTO m1m3_Application_items VALUES (4,"status","short",16,"",0.054,"","","");
