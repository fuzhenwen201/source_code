CREATE TABLE error_message (
    id        INTEGER  PRIMARY KEY
                       UNIQUE
                       NOT NULL,
    queuename VARCHAR  NOT NULL,
    tag       INTEGER  NOT NULL,
    module    VARCHAR  NOT NULL,
    errno     INTEGER  NOT NULL,
    errmsg    VARCHAR  NOT NULL,
    details   VARCHAR  NOT NULL,
    time      DATETIME NOT NULL
);



CREATE TABLE tag_error (
    id            INTEGER PRIMARY KEY
                          NOT NULL,
    responsequeue VARCHAR,
    tag           INTEGER,
    messagetype   VARCHAR,
    body
);