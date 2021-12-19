/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 50730
 Source Host           : localhost:3306
 Source Schema         : jwxt_gzhu

 Target Server Type    : MySQL
 Target Server Version : 50730
 File Encoding         : 65001

 Date: 12/01/2021 13:18:44
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for admin
-- ----------------------------
DROP TABLE IF EXISTS `admin`;
CREATE TABLE `admin`  (
  `AdminID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `Pwd` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `Email` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  PRIMARY KEY (`AdminID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for class
-- ----------------------------
DROP TABLE IF EXISTS `class`;
CREATE TABLE `class`  (
  `ClassName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `TeaID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `BaseTable` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  PRIMARY KEY (`ClassName`) USING BTREE,
  INDEX `FK_Reference_3`(`TeaID`) USING BTREE,
  CONSTRAINT `FK_Reference_3` FOREIGN KEY (`TeaID`) REFERENCES `teacher` (`TeaID`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for course
-- ----------------------------
DROP TABLE IF EXISTS `course`;
CREATE TABLE `course`  (
  `CourID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `CourName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `CourType` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `ExamType` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `Credit` varchar(11) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `CourHour` int(11) DEFAULT NULL,
  PRIMARY KEY (`CourID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for coursegroop
-- ----------------------------
DROP TABLE IF EXISTS `coursegroop`;
CREATE TABLE `coursegroop`  (
  `CGID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `CGName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `TeaID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `TeaManager` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  PRIMARY KEY (`CGID`) USING BTREE,
  INDEX `FK_Reference_10`(`TeaID`) USING BTREE,
  CONSTRAINT `FK_Reference_10` FOREIGN KEY (`TeaID`) REFERENCES `teacher` (`TeaID`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for coursepool
-- ----------------------------
DROP TABLE IF EXISTS `coursepool`;
CREATE TABLE `coursepool`  (
  `CPID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '课程池id',
  `CourID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '课程id',
  `TeaID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '教师id',
  `OpenClass` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '开设班级',
  `OpenNum` int(11) DEFAULT NULL COMMENT '课程选课人数',
  `Place` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '教学地点',
  `Time` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '上课时间',
  `OpenNumMin` int(11) DEFAULT NULL COMMENT '课程最小开设人数',
  `OpenNunMax` int(11) DEFAULT NULL COMMENT '课程容量',
  PRIMARY KEY (`CPID`) USING BTREE,
  INDEX `FK_Reference_6`(`CourID`) USING BTREE,
  INDEX `FK_Reference_8`(`TeaID`) USING BTREE,
  CONSTRAINT `FK_Reference_6` FOREIGN KEY (`CourID`) REFERENCES `course` (`CourID`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `FK_Reference_8` FOREIGN KEY (`TeaID`) REFERENCES `teacher` (`TeaID`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for sc
-- ----------------------------
DROP TABLE IF EXISTS `sc`;
CREATE TABLE `sc`  (
  `StuID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '学生id',
  `CPID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '课程池id',
  `StuGrade` int(11) DEFAULT NULL COMMENT '学生成绩',
  INDEX `StuID`(`StuID`) USING BTREE,
  INDEX `CPID`(`CPID`) USING BTREE,
  CONSTRAINT `CPID` FOREIGN KEY (`CPID`) REFERENCES `coursepool` (`CPID`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `StuID` FOREIGN KEY (`StuID`) REFERENCES `student` (`StuID`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for student
-- ----------------------------
DROP TABLE IF EXISTS `student`;
CREATE TABLE `student`  (
  `StuID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `ClassName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `StuName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `StuSex` varchar(4) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `PersonTable` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `Pwd` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  `Email` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL,
  PRIMARY KEY (`StuID`) USING BTREE,
  INDEX `FK_Reference_1`(`ClassName`) USING BTREE,
  CONSTRAINT `FK_Reference_1` FOREIGN KEY (`ClassName`) REFERENCES `class` (`ClassName`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for teacher
-- ----------------------------
DROP TABLE IF EXISTS `teacher`;
CREATE TABLE `teacher`  (
  `TeaID` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '教师工号',
  `TeaName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '教师名称',
  `TeaTable` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '教师个人课表',
  `IsDean` tinyint(1) DEFAULT NULL COMMENT '是否是系主任',
  `Pwd` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '密码',
  `Email` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci DEFAULT NULL COMMENT '邮箱',
  PRIMARY KEY (`TeaID`) USING BTREE,
  INDEX `TeaName`(`TeaName`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

SET FOREIGN_KEY_CHECKS = 1;
