CREATE DATABASE test_amy;
CREATE USER 'amy'@'localhost' IDENTIFIED BY 'amy';
GRANT ALL PRIVILEGES ON test_amy.* TO 'amy'@'localhost';
FLUSH PRIVILEGES;
