CREATE DATABASE IF NOT EXISTS tienda_adaptativa CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE tienda_adaptativa;

CREATE TABLE entities (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(150) NOT NULL,
    active TINYINT DEFAULT 1
);

CREATE TABLE optionsGroups (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    active TINYINT DEFAULT 1,
    name VARCHAR(100) NOT NULL
);

CREATE TABLE options (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    active TINYINT DEFAULT 1,
    value VARCHAR(100) NOT NULL
);

CREATE TABLE validOptions (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    optionsId BIGINT NOT NULL,
    optionsGroupsId BIGINT NOT NULL,
    FOREIGN KEY (optionsId) REFERENCES options(id) ON DELETE CASCADE,
    FOREIGN KEY (optionsGroupsId) REFERENCES optionsGroups(id) ON DELETE CASCADE
);

CREATE TABLE entityHasMultipleOptions (
    entityId BIGINT NOT NULL,
    validOptionsId BIGINT NOT NULL,
    UNIQUE KEY unq_entity_valid_opt (entityId, validOptionsId),
    FOREIGN KEY (entityId) REFERENCES entities(id) ON DELETE CASCADE,
    FOREIGN KEY (validOptionsId) REFERENCES validOptions(id) ON DELETE CASCADE
);

CREATE TABLE entityHasUniqueOptions (
    entityId BIGINT NOT NULL,
    validOptionsId BIGINT NOT NULL,
    optionsGroupsId BIGINT NOT NULL,
    UNIQUE KEY unq_entity_group (entityId, optionsGroupsId),
    FOREIGN KEY (entityId) REFERENCES entities(id) ON DELETE CASCADE,
    FOREIGN KEY (validOptionsId) REFERENCES validOptions(id) ON DELETE CASCADE,
    FOREIGN KEY (optionsGroupsId) REFERENCES optionsGroups(id) ON DELETE CASCADE
);

INSERT INTO optionsGroups (id, name) VALUES (1, 'Talle (Ropa)'), (2, 'Color'), (3, 'Voltaje (Tecnología)');

INSERT INTO options (id, value) VALUES 
(1, 'L (Large)'), (2, 'Rojo'), (3, 'Azul'), (4, '220V');

INSERT INTO validOptions (id, optionsId, optionsGroupsId) VALUES 
(101, 1, 1),
(102, 2, 2),
(103, 3, 2),
(104, 4, 3);

INSERT INTO entities (id, name) VALUES (50, 'Campera Impermeable Urbana');

INSERT INTO entityHasUniqueOptions (entityId, validOptionsId, optionsGroupsId) 
VALUES (50, 101, 1);

INSERT INTO entityHasMultipleOptions (entityId, validOptionsId) 
VALUES 
(50, 102), 
(50, 103); 

SELECT 
    e.name AS producto,
    og.name AS grupo_opcion,
    o.value AS valor_opcion,
    'Única' AS tipo
FROM entities e
JOIN entityHasUniqueOptions eu ON e.id = eu.entityId
JOIN validOptions vo ON eu.validOptionsId = vo.id
JOIN options o ON vo.optionsId = o.id
JOIN optionsGroups og ON vo.optionsGroupsId = og.id
WHERE e.id = 50

UNION ALL

SELECT 
    e.name AS producto,
    og.name AS grupo_opcion,
    o.value AS valor_opcion,
    'Múltiple' AS tipo
FROM entities e
JOIN entityHasMultipleOptions em ON e.id = em.entityId
JOIN validOptions vo ON em.validOptionsId = vo.id
JOIN options o ON vo.optionsId = o.id
JOIN optionsGroups og ON vo.optionsGroupsId = og.id
WHERE e.id = 50;