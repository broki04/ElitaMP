-- phpMyAdmin SQL Dump
-- version 5.1.0
-- https://www.phpmyadmin.net/
--
-- Host: i1.liveserver.pl
-- Czas generowania: 04 Sie 2022, 10:38
-- Wersja serwera: 10.5.15-MariaDB-0+deb11u1-log
-- Wersja PHP: 7.4.30

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Baza danych: `db_26970`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_accounts`
--

CREATE TABLE `p4s_accounts` (
  `id` int(11) NOT NULL,
  `nick` text NOT NULL,
  `password` text NOT NULL,
  `admin` tinyint(4) NOT NULL DEFAULT 0,
  `admin_level` tinyint(4) NOT NULL DEFAULT 0,
  `money` int(11) NOT NULL DEFAULT 0,
  `admin_pass` text NOT NULL DEFAULT '@NULL',
  `ip_reg` text NOT NULL,
  `ip_last` text NOT NULL,
  `serial_reg` text NOT NULL,
  `serial_last` text NOT NULL,
  `date_reg` text NOT NULL,
  `date_last` text NOT NULL,
  `suicides` int(11) NOT NULL DEFAULT 0,
  `visits` int(11) NOT NULL DEFAULT 0,
  `session` int(11) NOT NULL DEFAULT 0,
  `admin_online` int(11) NOT NULL DEFAULT 0,
  `admin_points` int(11) NOT NULL DEFAULT 0,
  `admin_penalty` text NOT NULL DEFAULT '0, 0, 0, 0, 0',
  `event_points` int(11) NOT NULL DEFAULT 0,
  `event_count` int(11) NOT NULL DEFAULT 0,
  `hud` text NOT NULL,
  `gang` int(11) NOT NULL DEFAULT -1,
  `gang_rank` text NOT NULL DEFAULT 'Swiezy',
  `gang_rank_permission` text NOT NULL DEFAULT '0,0,0,0,0',
  `gang_rank_color` int(11) NOT NULL DEFAULT 0,
  `score` int(11) NOT NULL DEFAULT 0,
  `score_needed` int(11) NOT NULL DEFAULT 500,
  `level` int(11) NOT NULL DEFAULT 1,
  `duel_rank` smallint(6) NOT NULL DEFAULT 0,
  `duel_points` int(11) NOT NULL DEFAULT 0,
  `skin` int(11) NOT NULL DEFAULT 0,
  `settings` text NOT NULL DEFAULT '1, 1',
  `nick_timer` int(11) NOT NULL DEFAULT 0,
  `secret_question` text NOT NULL DEFAULT '@NULL',
  `secret_answer` text NOT NULL DEFAULT '@NULL',
  `secret_pincode` text NOT NULL DEFAULT '@NULL',
  `spawn_data` tinyint(4) NOT NULL DEFAULT 0,
  `last_pos` text NOT NULL DEFAULT '0.0, 0.0, 0.0, 0.0',
  `last_int` int(11) NOT NULL DEFAULT 0,
  `staly_pos` text NOT NULL DEFAULT '0.0, 0.0, 0.0, 0.0',
  `staly_int` int(11) NOT NULL DEFAULT 0,
  `premium` int(11) NOT NULL DEFAULT 0,
  `own_rank` text NOT NULL DEFAULT '@NULL',
  `own_rank_color` int(11) NOT NULL DEFAULT 0,
  `mute` int(11) NOT NULL DEFAULT 0,
  `warns` int(11) NOT NULL DEFAULT 0,
  `jail` int(11) NOT NULL DEFAULT 0,
  `glitch` float NOT NULL DEFAULT 100,
  `weapon` text NOT NULL DEFAULT '0, 24, 25, 29, 0, 0',
  `arena_kills` text NOT NULL DEFAULT '0, 0, 0, 0',
  `arena_deaths` text NOT NULL DEFAULT '0, 0, 0, 0',
  `level_points` int(11) NOT NULL DEFAULT 0,
  `ach_count` text NOT NULL DEFAULT '0, 0, 0, 0, 0',
  `ach_level` text NOT NULL DEFAULT '0, 0, 0, 0, 0',
  `ach_status` text NOT NULL DEFAULT '0, 0, 0, 0, 0',
  `ach_done` text NOT NULL DEFAULT '0, 0, 0, 0, 0',
  `daily` int(11) NOT NULL DEFAULT -1,
  `daily_status` int(11) NOT NULL DEFAULT 0,
  `daily_done` tinyint(4) NOT NULL DEFAULT 0,
  `daily_reward` text NOT NULL DEFAULT '0,0',
  `job_level` text NOT NULL DEFAULT '1, 1, 1, 1',
  `job_score` text NOT NULL DEFAULT '0, 0, 0, 0',
  `job_reward` text NOT NULL DEFAULT '0,0,0',
  `job_ulep` text NOT NULL DEFAULT '0,0,0',
  `job_points` int(11) NOT NULL DEFAULT 0,
  `ulepszenia` text NOT NULL DEFAULT '0,0,0,0',
  `ulepszenia_level` text NOT NULL DEFAULT '0,0,0,0',
  `bonus_vcxz` tinyint(2) NOT NULL DEFAULT 0,
  `player_gang_loan` int(11) NOT NULL DEFAULT 0,
  `fish_rode` smallint(6) NOT NULL DEFAULT -1,
  `fish_zylka` float NOT NULL DEFAULT 0,
  `fish_bait` text NOT NULL DEFAULT '0,0,0,0,0',
  `fish_ulep` text NOT NULL DEFAULT '0,0,0,0,0',
  `license` text NOT NULL DEFAULT '0, 0, 0',
  `pickaxe` int(11) NOT NULL DEFAULT -1,
  `pickaxe_health` text NOT NULL DEFAULT '0.0, 0.0',
  `pickaxe_name` text NOT NULL DEFAULT 'Kilof górniczy',
  `mine_weight` text NOT NULL DEFAULT '0,0,0,0,0,0,0,0,0,0,0',
  `mine_bag` int(11) NOT NULL DEFAULT 0,
  `mine_points` int(11) NOT NULL DEFAULT 0,
  `chat` smallint(6) NOT NULL DEFAULT 0,
  `truck_dist` float NOT NULL DEFAULT 0,
  `fightstyle` int(11) NOT NULL DEFAULT 0,
  `draw_visible` int(11) NOT NULL DEFAULT 1,
  `bank` int(11) NOT NULL DEFAULT 0,
  `fortune_block` int(11) NOT NULL DEFAULT 0,
  `casino_tokens` int(11) NOT NULL DEFAULT 100
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_accounts`
--

INSERT INTO `p4s_accounts` (`id`, `nick`, `password`, `admin`, `admin_level`, `money`, `admin_pass`, `ip_reg`, `ip_last`, `serial_reg`, `serial_last`, `date_reg`, `date_last`, `suicides`, `visits`, `session`, `admin_online`, `admin_points`, `admin_penalty`, `event_points`, `event_count`, `hud`, `gang`, `gang_rank`, `gang_rank_permission`, `gang_rank_color`, `score`, `score_needed`, `level`, `duel_rank`, `duel_points`, `skin`, `settings`, `nick_timer`, `secret_question`, `secret_answer`, `secret_pincode`, `spawn_data`, `last_pos`, `last_int`, `staly_pos`, `staly_int`, `premium`, `own_rank`, `own_rank_color`, `mute`, `warns`, `jail`, `glitch`, `weapon`, `arena_kills`, `arena_deaths`, `level_points`, `ach_count`, `ach_level`, `ach_status`, `ach_done`, `daily`, `daily_status`, `daily_done`, `daily_reward`, `job_level`, `job_score`, `job_reward`, `job_ulep`, `job_points`, `ulepszenia`, `ulepszenia_level`, `bonus_vcxz`, `player_gang_loan`, `fish_rode`, `fish_zylka`, `fish_bait`, `fish_ulep`, `license`, `pickaxe`, `pickaxe_health`, `pickaxe_name`, `mine_weight`, `mine_bag`, `mine_points`, `chat`, `truck_dist`, `fightstyle`, `draw_visible`, `bank`, `fortune_block`, `casino_tokens`) VALUES
(1, 'psg.broki.)', '$2y$12$0DXcrqEKQ3PEbk/10l/9CuO.QanL/SwkJ8lThXEulp6KOFxrxPwKi', 4, 2, 40014541, 'lol123', '127.0.0.1', '127.0.0.1', '9D98CAFE8E49FEF8FA8F98DEF494C889844CCA8D', '9D98CAFE8E49FEF8FA8F98DEF494C889844CCA8D', '02.04.2022, 17:49', '27.07.2022, 11:36', 0, 307, 27755, 10557, 0, '0, 0, 0, 0, 0', 0, 0, '548580095,548580095,548580095,548580095,548580095,548580095,548580095,548580095,548580095,548580095,548580095,548580095,548580095,548580095,0', 1, 'Za³o¿yciel', '1,1,1,1', -443336449, 1716, 8000, 27, 0, 7, 137, '1, 1', 0, 'czy jestem koksem', '2137bool', '8850', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1657537185, '@NULL', 0, 0, 0, 0, 2.048, '0,24,26,29,0,0', '0,0,0,0', '0,0,0,0', 13, '5,5,0,0,14,0,0,0,0,0,0,1,1,10,0,0,6,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1', '0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1', '5,5,0,0,9,0,0,0,0,0,0,1,1,0,0,0,6,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1,1,1,1,1,1,2', '3,0,0,0,225,111,44', '0,0,0', '0,0,0', 1, '0,0,0,0,1', '0,0,0,0', 0, 0, 3, 96.3, '0,0,0,0,46', '1,1,0,0', '1, 0, 0', 1, '94.16, 100.00', 'Kilof górniczy', '0,0,0,0,5,0,0,0,0', 1, 0, 0, 1, 3, 1, -300, 1658646368, 7927),
(2, 'Vester_', '$2y$12$JWOCCqC4z8m9Exc27lyaNuCbhRyqhRo28dSKv.1CUX0wxyBhp29hO', 0, 0, 12271550, '@NULL', '84.205.8.106', '84.205.8.106', '9D0F4CDED9A5C485DC99D8CDE8CE4994484DD49C', '9D0F4CDED9A5C485DC99D8CDE8CE4994484DD49C', '19.04.2022, 17:27', '19.04.2022, 17:56', 0, 0, 1708, 0, 0, '0, 0, 0, 0, 0', 0, 0, '-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721,-1782910721', 1, 'Swiezy', '0,0,0,0', -946798337, 102, 1500, 3, 0, 0, 0, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1651505238, '@NULL', 0, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 2, '2,3,0,0,0,0,0,0,0,0,0,7,0,17', '0,0,0,0,0,0,0,0,0,0,0,0,0,1', '2,3,0,0,0,0,0,0,0,0,0,7,0,7', '0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1,1,1,1,1,1', '34,0,0,0,0,0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, 3, 176.1, '0,0,0,0,26', '1,1,1,1', '0, 0, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 0, 100),
(3, 'mixie', '$2y$12$YGmgIj9pU0vFTRPI20q7sOJIFaXi7vYIAmxI308eJMjnWIM8DEINq', 0, 0, 10595511, '@NULL', '83.20.171.128', '83.20.171.128', 'C49EC0DE9DD9CDAD5FAA08F5D09E9548FA589889', 'C49EC0DE9DD9CDAD5FAA08F5D09E9548FA589889', '07.05.2022, 16:13', '07.05.2022, 16:50', 0, 1, 2154, 0, 0, '0, 0, 0, 0, 0', 0, 0, '-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505,-1061109505', 2, 'Boss', '1,1,1,1', 2118768639, 218, 1000, 2, 0, 0, 0, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1652192008, 'mixie', 1073774847, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 1, '0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,1,0', '0,0,0,0', 0, 0, -1, 0, '0,0,0,0,0', '0,0,0,0,0', '1, 1, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 0, 100),
(4, 'vstr', '$2y$12$N8un7hapqpQwhl5wns8wNuRBcsFi83nApB5RLwYdrg6rnJEncgjA6', 0, 0, 0, '@NULL', '84.205.8.106', '84.205.8.106', '9D0F4CDED9A5C485DC99D8CDE8CE4994484DD49C', '9D0F4CDED9A5C485DC99D8CDE8CE4994484DD49C', '08.05.2022, 21:50', '08.05.2022, 21:52', 0, 0, 112, 0, 0, '0, 0, 0, 0, 0', 0, 0, '2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351,2055012351', -1, 'Swiezy', '0,0,0,0,0', 0, 0, 500, 1, 0, 0, 192, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1652298612, '@NULL', 0, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 0, '0,0,0,0,0,0,0,0,0,0,0,7,0,17,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,7,0,7,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, -1, 0, '0,0,0,0,0', '0,0,0,0,0', '0, 0, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 0, 100),
(5, 'speed_sewyy', '$2y$12$DBEfanifdfysGFAkKV5dQO6CNrDJbMLlHV.qekDBH/aaU2dcZTJIq', 3, 1, 16819, 'jebaclou', '31.42.0.54', '31.42.0.54', 'D95EDC4DDC989F4440DF5890E449954C4CC9898D', 'D95EDC4DDC989F4440DF5890E449954C4CC9898D', '28.06.2022, 19:42', '28.06.2022, 20:02', 0, 2, 288, 0, 0, '0, 0, 0, 0, 0', 0, 0, '-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641,-1347440641', -1, 'Swiezy', '0,0,0,0,0', 0, 0, 1000, 2, 0, 0, 0, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1656697322, '@NULL', 0, 0, 0, 0, 1.143, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 0, '1,3,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '1,3,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, 3, 300, '0,0,0,0,10', '1,1,1,1', '0, 0, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 0, 100),
(6, 'negro_da_polonia', '$2y$12$/xuwX969BkS9aF/5AAYGpeMDLK/iP6Cw/n3bm9xDT/hm//E4BLPkK', 0, 0, 5020967, '@NULL', '31.42.0.54', '31.42.0.54', 'D95EDC4DDC989F4440DF5890E449954C4CC9898D', 'D95EDC4DDC989F4440DF5890E449954C4CC9898D', '07.07.2022, 20:13', '07.07.2022, 20:47', 0, 5, 1131, 0, 0, '0, 0, 0, 0, 0', 0, 0, '2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639,2118768639', 1, 'waifu', '0,1,0,0', 1644168703, 94, 500, 1, 0, 0, 141, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1658427232, '@NULL', 0, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 0, '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, -1, 0, '0,0,0,0,0', '0,0,0,0,0', '0, 0, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 1657304191, 77),
(7, 'mbp', '$2y$12$riQ5oEIKrINNyIOd7dNWx.Heq5cJWDonwu.l6uPd5mL4pNZKOCUSS', 0, 0, 5000000, '@NULL', '178.235.181.10', '178.235.181.10', '48CF9E9E09958DD548CE9FE4D88D8E94CDFC4A80', '48CF9E9E09958DD548CE9FE4D88D8E94CDFC4A80', '07.07.2022, 20:14', '07.07.2022, 20:21', 0, 1, 374, 0, 0, '0, 0, 0, 0, 0', 0, 0, '407714815,407714815,407714815,407714815,407714815,407714815,407714815,407714815,407714815,407714815,407714815,407714815,407714815,407714815', 3, 'W³aœciciel', '1,1,1,1', 16777215, 16, 500, 1, 0, 45, 93, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1658081696, '@NULL', 0, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 0, '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, -1, 0, '0,0,0,0,0', '0,0,0,0,0', '0, 0, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 1657304325, 890),
(8, 'vcxzz', '$2y$12$b6NfkKfZnvP.k71K55YGNOb2KLWfxRhYIKK3xtr.Io.7GYbREeHrS', 0, 0, 28725, '@NULL', '81.161.204.226', '81.161.204.226', 'C40CD09DC89DF59D9D998E9EDA8D949A4C95944D', 'C40CD09DC89DF59D9D998E9EDA8D949A4C95944D', '07.07.2022, 20:20', '07.07.2022, 20:34', 0, 1, 775, 0, 0, '0, 0, 0, 0, 0', 0, 0, '16711935,16711935,16711935,16711935,16711935,16711935,16711935,16711935,16711935,16711935,16711935,16711935,16711935,16711935', 3, 'Swiezy', '0,0,0,0', 1884442111, 77, 500, 1, 0, 12, 230, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1657477244, '@NULL', 0, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 0, '2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0', '2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, -1, 0, '0,0,0,0,0', '0,0,0,0,0', '0, 0, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 0, 100),
(9, 'janek420', '$2y$12$a5H.2zxACogCNHTelonK3O/fFwbmdr/1bS2qQlI28eK/LcNFilGEu', 0, 0, 0, '@NULL', '213.134.161.167', '213.134.161.167', '9F8D48449944895EDA9E448940C5E9098F0D4C59', '9F8D48449944895EDA9E448940C5E9098F0D4C59', '22.07.2022, 12:50', '22.07.2022, 12:50', 0, 0, 0, 0, 0, '0, 0, 0, 0, 0', 0, 0, '', -1, 'Swiezy', '0,0,0,0,0', 0, 0, 500, 1, 0, 0, 0, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 0, '@NULL', 0, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0, 0, 0, 0', '0, 0, 0, 0', 0, '0, 0, 0, 0, 0', '0, 0, 0, 0, 0', '0, 0, 0, 0, 0', '0, 0, 0, 0, 0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, -1, 0, '0,0,0,0,0', '0,0,0,0,0', '0, 0, 0', -1, '0.0, 0.0', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 0, 100),
(10, 'brokitest', '$2y$12$OBVxDvk8npKzcgRxvjzTh.tmqlS/f3ZwPQzywpFXl73e7kbpoGXCq', 0, 0, 0, '@NULL', '127.0.0.1', '127.0.0.1', '9D98CAFE8E49FEF8FA8F98DEF494C889844CCA8D', '9D98CAFE8E49FEF8FA8F98DEF494C889844CCA8D', '25.07.2022, 16:33', '27.07.2022, 11:36', 0, 16, 1376, 0, 0, '0, 0, 0, 0, 0', 0, 0, '-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441,-602653441', -1, 'Swiezy', '0,0,0,0,0', 0, 63, 500, 1, 0, 0, 230, '1, 1', 0, '@NULL', '@NULL', '@NULL', 0, '0.0, 0.0, 0.0, 0.0', 0, '0.0, 0.0, 0.0, 0.0', 0, 1659018831, '@NULL', 0, 0, 0, 0, 100, '0, 24, 25, 29, 0, 0', '0,0,0,0', '0,0,0,0', 0, '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', '0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0', -1, 0, 0, '0,0', '1, 1, 1, 1', '0, 0, 0, 0', '0,0,0', '0,0,0', 0, '0,0,0,0', '0,0,0,0', 0, 0, -1, 0, '0,0,0,0,0', '0,0,0,0,0', '0, 0, 0', -1, '0.00, 0.00', 'Kilof górniczy', '0,0,0,0,0,0,0,0,0', 0, 0, 0, 0, 0, 1, 0, 0, 100);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_bans`
--

CREATE TABLE `p4s_bans` (
  `id` int(11) NOT NULL,
  `userid` int(11) NOT NULL,
  `adminid` int(11) NOT NULL,
  `reason` text NOT NULL,
  `date` text NOT NULL,
  `ip` text NOT NULL,
  `gpci` text NOT NULL,
  `expire` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_business`
--

CREATE TABLE `p4s_business` (
  `id` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `name` text NOT NULL,
  `ownerid` int(11) NOT NULL,
  `position` text NOT NULL,
  `cost` int(11) NOT NULL DEFAULT 0,
  `reward` int(11) NOT NULL DEFAULT 0,
  `price` int(11) NOT NULL DEFAULT 10
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_business`
--

INSERT INTO `p4s_business` (`id`, `type`, `name`, `ownerid`, `position`, `cost`, `reward`, `price`) VALUES
(1, 0, 'Four Dragons', 1, '2019.316894, 1007.765258, 10.820312', 10000000, 3316, 10),
(2, 0, 'Royal Casino', -1, '2090.012207, 1451.606201, 10.820312', 10000000, 0, 10),
(3, 0, 'Caligula Casino', -1, '2196.966552, 1677.139892, 12.367187', 10000000, 0, 10),
(4, 0, 'Babilon Casino', -1, '2165.526367, 2164.119384, 10.820312', 7000000, 0, 10),
(5, 0, 'Poker Casino', -1, '2169.857421, 2115.591308, 10.820312', 8000000, 0, 10),
(6, 0, 'Vegas Casino', -1, '2315.774902, 2116.537353, 10.828125', 8500000, 0, 10),
(7, 0, 'Elite Casino', -1, '2331.146484, 2165.517578, 10.828125', 10000000, 0, 10),
(8, 0, 'The Light Casino', -1, '2217.578613, 2123.589111, 10.820312', 7500000, 0, 10);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_config`
--

CREATE TABLE `p4s_config` (
  `records` varchar(50) NOT NULL DEFAULT '0, 0, 0, 0, 0, 0',
  `connects` int(11) NOT NULL DEFAULT 0,
  `messages` int(11) NOT NULL DEFAULT 0,
  `bans` int(11) NOT NULL DEFAULT 0,
  `kicks` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_config`
--

INSERT INTO `p4s_config` (`records`, `connects`, `messages`, `bans`, `kicks`) VALUES
('4,0,2,0,0,0,0,1,0,0,0,0,0,2,0,4,3', 27, 13, 0, 0);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_fun_rc`
--

CREATE TABLE `p4s_fun_rc` (
  `id` int(11) NOT NULL,
  `name` text NOT NULL,
  `modelid` int(11) NOT NULL DEFAULT 0,
  `position` text NOT NULL,
  `checkpoints` int(11) NOT NULL DEFAULT 0,
  `record_id` int(11) NOT NULL DEFAULT -1,
  `record_score` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_fun_rc_cp`
--

CREATE TABLE `p4s_fun_rc_cp` (
  `id` int(11) NOT NULL,
  `arenaid` int(11) NOT NULL,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_fun_wg`
--

CREATE TABLE `p4s_fun_wg` (
  `id` int(11) NOT NULL,
  `name` text NOT NULL DEFAULT '@NULL',
  `zone` text NOT NULL DEFAULT '0.0, 0.0, 0.0, 0.0',
  `att` text NOT NULL DEFAULT '0.0, 0.0, 0.0, 0.0',
  `def` text NOT NULL DEFAULT '0.0, 0.0, 0.0, 0.0',
  `weap` text NOT NULL DEFAULT '-1, -1, -1'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_fun_wg`
--

INSERT INTO `p4s_fun_wg` (`id`, `name`, `zone`, `att`, `def`, `weap`) VALUES
(1, 'Doherty', '-2144.645752, 109.856705, -2000.645752, 317.856689', '-2079.408691, 285.594391, 35.595428, 213.105499', '-2024.645752, 173.856705, 28.840626, 358.589172', '24, 27, 31'),
(2, 'Stadion', '1290.497070, 2089.511963, 1434.497070, 2233.511963', '1410.436646, 2153.520508, 12.015625, 89.162292', '1334.087402, 2145.080078, 11.015625, 270.623291', '24, 29, 34');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_fun_wp`
--

CREATE TABLE `p4s_fun_wp` (
  `id` int(11) NOT NULL,
  `arenaid` int(11) NOT NULL DEFAULT -1,
  `record_uid` int(11) NOT NULL DEFAULT -1,
  `record_time` int(11) NOT NULL DEFAULT -1
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_fun_wp`
--

INSERT INTO `p4s_fun_wp` (`id`, `arenaid`, `record_uid`, `record_time`) VALUES
(1, 0, -1, 0),
(2, 1, -1, 0),
(3, 2, -1, 0),
(4, 3, -1, 0);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_gangs`
--

CREATE TABLE `p4s_gangs` (
  `id` int(11) NOT NULL,
  `name` text NOT NULL,
  `tag` text NOT NULL,
  `color` int(11) NOT NULL,
  `ownerid` int(11) NOT NULL,
  `slots` int(11) NOT NULL DEFAULT 10,
  `kills` int(11) NOT NULL DEFAULT 0,
  `deaths` int(11) NOT NULL DEFAULT 0,
  `respect` int(11) NOT NULL DEFAULT 0,
  `respect_needed` int(11) NOT NULL DEFAULT 100,
  `level` int(11) NOT NULL DEFAULT 1,
  `sparing` text NOT NULL DEFAULT '0, 0, 0',
  `budget` int(11) NOT NULL DEFAULT 0,
  `date_created` text NOT NULL,
  `default_rank` text NOT NULL DEFAULT 'Swiezy',
  `sparing_points` int(11) NOT NULL DEFAULT 0,
  `sparing_rank` smallint(6) NOT NULL DEFAULT 0,
  `ach_done` text NOT NULL DEFAULT '0, 0, 0, 0',
  `ach_status` text NOT NULL DEFAULT '0, 0, 0, 0',
  `ulepszenia` text NOT NULL DEFAULT '0, 0, 0, 0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_gangs`
--

INSERT INTO `p4s_gangs` (`id`, `name`, `tag`, `color`, `ownerid`, `slots`, `kills`, `deaths`, `respect`, `respect_needed`, `level`, `sparing`, `budget`, `date_created`, `default_rank`, `sparing_points`, `sparing_rank`, `ach_done`, `ach_status`, `ulepszenia`) VALUES
(1, 'testowy gang', 'tg', 1687547391, 1, 13, 6, 7, 4651, 5400, 54, '0, 0, 0', 0, '08.07.2022, 12:22', 'Swiezy', 21, 0, '0, 0, 0, 0', '0, 0, 0, 0', '5,0,0');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_gangs_bases`
--

CREATE TABLE `p4s_gangs_bases` (
  `id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL DEFAULT 'Baza gangowa',
  `ownerid` int(11) NOT NULL DEFAULT -1,
  `cost` int(11) NOT NULL DEFAULT 0,
  `settings` text NOT NULL DEFAULT '0, 0',
  `sell` tinyint(4) NOT NULL DEFAULT 0,
  `sell_time` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_gangs_bases`
--

INSERT INTO `p4s_gangs_bases` (`id`, `name`, `ownerid`, `cost`, `settings`, `sell`, `sell_time`) VALUES
(1, 'chuj dupa cycki', -1, 500000, '0, 0', 0, 0),
(2, 'Baza gangowa', -1, 300000, '0, 0', 0, 0),
(3, 'Baza gangowa', -1, 250000, '0, 0', 0, 0),
(4, 'Baza gangowa', -1, 700000, '0, 0', 0, 0);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_gangs_basesObj`
--

CREATE TABLE `p4s_gangs_basesObj` (
  `id` int(11) NOT NULL,
  `baseid` int(11) NOT NULL,
  `listitem` int(11) NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `rx` float NOT NULL DEFAULT 0,
  `ry` float NOT NULL DEFAULT 0,
  `rz` float NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_gangs_zones`
--

CREATE TABLE `p4s_gangs_zones` (
  `id` int(11) NOT NULL,
  `ownerid` int(11) NOT NULL DEFAULT -1,
  `position` text NOT NULL,
  `checkpoint` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_gangs_zones`
--

INSERT INTO `p4s_gangs_zones` (`id`, `ownerid`, `position`, `checkpoint`) VALUES
(1, -1, '2206.5, -1726.5, 2540.5, -1607.5', '2430.9019,-1678.5236,13.3153,88.6472'),
(2, -1, '2206.5, -1840.5, 2415.5, -1726.5', '2370.8323,-1787.2920,13.1143,178.8159'),
(3, -1, '2415.5, -1935.5, 2540.5, -1726.5', '2449.7368,-1764.6392,13.1514,259.4205'),
(4, -1, '2104.5, -1754.5, 2206.5, -1546.5', '2166.6194,-1684.6042,14.6537,87.5227'),
(5, -1, '1947.5, -1754.5, 2104.5, -1607.5', '2041.1368,-1714.4314,13.1147,181.1570'),
(6, -1, '1809.5, -1754.5, 1947.5, -1607.5', '1923.0031,-1648.4956,13.1132,359.2526'),
(7, -1, '1809.5, -1607.5, 2104.5, -1461.5', '1959.7410,-1568.6182,13.1643,269.6422'),
(8, -1, '1809.5, -1939.5, 1961.5, -1754.5', '1880.1860,-1795.3854,13.1132,89.3770'),
(9, -1, '1676.5, -2058.5, 1809.5, -1812.5', '1766.4618,-1938.9575,13.1384,87.8113'),
(10, -1, '1961.5, -1944.5, 2095.5, -1754.5', '2058.0793,-1917.8180,13.1279,327.4473'),
(11, -1, '2095.5, -1946.5, 2206.5, -1754.5', '2178.6594,-1909.7542,13.0783,187.8568'),
(12, -1, '1809.5, -2045.5, 1961.5, -1939.5', '1882.9945,-2003.2601,13.1132,5.1170'),
(13, -1, '1809.5, -2158.5, 1961.5, -2045.5', '1865.9224,-2101.2046,13.1043,275.9525'),
(14, -1, '1644.5, -2158.5, 1809.5, -2058.5', '1671.2909,-2113.0579,13.1145,91.0660'),
(15, -1, '2206.5, -1975.5, 2415.5, -1840.5', '2338.1147,-1913.3983,13.1754,271.4084'),
(16, -1, '2540.5, -2037.5, 2632.5, -1607.5', '2572.2578,-1739.7567,13.1132,268.4761'),
(17, -1, '2429.5, -1607.5, 2632.5, -1477.5', '2519.6150,-1533.3483,23.0642,169.9957'),
(18, -1, '2206.5, -1607.5, 2429.5, -1477.5', '2316.4961,-1527.5541,24.9115,273.8651'),
(19, -1, '2104.5, -1546.5, 2206.5, -1387.5', '2170.8872,-1472.0497,23.5514,3.4085'),
(20, -1, '2206.5, -1477.5, 2451.5, -1387.5', '2328.7744,-1411.6351,23.5588,184.1522'),
(21, -1, '2103.5, -1387.5, 2308.5, -1229.5', '2229.5352,-1328.0834,23.5508,180.4475'),
(22, -1, '1853.5, -1461.5, 2103.5, -1335.5', '1886.0115,-1374.4996,13.1384,271.1929'),
(23, -1, '1851.5, -1335.5, 2103.5, -1132.5', '1890.8618,-1170.8782,23.7219,298.2074'),
(24, -1, '2103.5, -1229.5, 2308.5, -1108.5', '2154.8420,-1173.9431,23.3891,359.2439'),
(25, -1, '2308.5, -1387.5, 2372.5, -1142.5', '2340.7046,-1315.2375,23.6432,181.9140'),
(26, -1, '2372.5, -1387.5, 2451.5, -1142.5', '2429.2498,-1241.1018,23.9115,264.6570'),
(27, -1, '2451.5, -1477.5, 2576.5, -1255.5', '2540.7937,-1344.3785,31.7359,356.8141'),
(28, -1, '2451.5, -1255.5, 2634.5, -1142.5', '2578.0706,-1192.2202,61.4180,358.3139'),
(29, -1, '1851.5, -1132.5, 2103.5, -1032.5', '1995.2944,-1094.9080,24.5205,78.1205'),
(30, -1, '2576.5, -1477.5, 2715.5, -1255.5', '2659.3162,-1429.0438,30.0413,172.3446'),
(31, -1, '2103.5, -1108.5, 2308.5, -959.5', '2237.4343,-1045.9386,54.7254,225.9461'),
(32, -1, '1925.5, -1032.5, 2103.5, -932.5', '1986.7445,-1002.6160,28.9830,107.0902'),
(33, -1, '2308.5, -1142.5, 2634.5, -978.5', '2513.9128,-1062.3051,69.1211,274.5461'),
(34, -1, '2634.5, -1255.5, 2716.5, -1029.5', '2683.7810,-1106.0232,68.9429,172.3933'),
(35, -1, '2415.5, -2038.5, 2540.5, -1935.5', '2455.6570,-1972.5145,13.1147,355.4598'),
(36, -1, '2632.5, -1647.5, 2715.5, -1477.5', '2675.6018,-1552.0090,22.1416,1.5498'),
(37, -1, '2632.5, -1896.5, 2870.5, -1647.5', '2813.1570,-1722.3807,9.5216,17.8439'),
(38, -1, '2632.5, -2041.5, 2832.5, -1896.5', '2777.9231,-2012.3762,13.1225,266.7133'),
(39, -1, '2715.5, -1485.5, 2880.5, -1255.5', '2790.9263,-1394.0405,20.9898,214.8642'),
(40, -1, '2715.5, -1647.5, 2890.5, -1485.5', '2808.4111,-1559.8860,10.6615,247.5381'),
(41, -1, '2716.5, -1255.5, 2835.5, -1051.5', '2807.6943,-1185.0222,24.9406,90.6664');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_privcars`
--

CREATE TABLE `p4s_privcars` (
  `id` int(11) NOT NULL,
  `ownerid` int(11) NOT NULL,
  `modelid` int(11) NOT NULL,
  `engine` smallint(6) NOT NULL DEFAULT 0,
  `gas` tinyint(4) NOT NULL DEFAULT 0,
  `gas_enabled` tinyint(4) NOT NULL DEFAULT 0,
  `position` text NOT NULL,
  `etykieta` text NOT NULL DEFAULT '0,0,0,0',
  `name` text NOT NULL DEFAULT 'PRYWATNY POJAZD',
  `color` text NOT NULL DEFAULT '-1, -1',
  `comp` text NOT NULL DEFAULT '0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0',
  `paintjob` int(11) NOT NULL DEFAULT 0,
  `dist` float NOT NULL DEFAULT 0,
  `fuel` text NOT NULL DEFAULT '0.0, 0.0',
  `fuel_type` tinyint(4) NOT NULL DEFAULT 0,
  `fuel_amount` float NOT NULL DEFAULT 30,
  `fuel_have` text NOT NULL DEFAULT '1,0,0',
  `register_color` int(11) NOT NULL,
  `register_name` text NOT NULL DEFAULT 'ELITA ',
  `date_created` text NOT NULL,
  `ulepszenia` text NOT NULL DEFAULT '0,0,0',
  `sell` tinyint(2) NOT NULL DEFAULT 0,
  `sell_cost` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_privcars`
--

INSERT INTO `p4s_privcars` (`id`, `ownerid`, `modelid`, `engine`, `gas`, `gas_enabled`, `position`, `etykieta`, `name`, `color`, `comp`, `paintjob`, `dist`, `fuel`, `fuel_type`, `fuel_amount`, `fuel_have`, `register_color`, `register_name`, `date_created`, `ulepszenia`, `sell`, `sell_cost`) VALUES
(1, 1, 411, 1, 0, 0, '2140.943847, 958.274047, 10.742792, 136.862319', '16711935,-397040897,-21761,2013528063', 'PRYWATNY POJAZD', '-1,-1', '0,0,0,0,0,0,0,0,0,0,0,0,0,0', 0, 0.398176, '29.886236, 0.000000', 0, 30, '1,0,0', -65281, 'ELITA ', '08.07.2022, 15:41', '0,0,0', 1, 1000000);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_privcars_attachments`
--

CREATE TABLE `p4s_privcars_attachments` (
  `id` int(11) NOT NULL,
  `privcarid` int(11) NOT NULL,
  `listitem` int(11) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `rx` float NOT NULL,
  `ry` float NOT NULL,
  `rz` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_teleports`
--

CREATE TABLE `p4s_teleports` (
  `id` int(11) NOT NULL,
  `name` text NOT NULL,
  `cmd` text NOT NULL,
  `category` tinyint(4) NOT NULL DEFAULT 0,
  `x` float NOT NULL DEFAULT 0,
  `y` float NOT NULL DEFAULT 0,
  `z` float NOT NULL DEFAULT 0,
  `a` float NOT NULL DEFAULT 0,
  `interior` int(11) NOT NULL DEFAULT 0,
  `spawn` tinyint(4) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_teleports`
--

INSERT INTO `p4s_teleports` (`id`, `name`, `cmd`, `category`, `x`, `y`, `z`, `a`, `interior`, `spawn`) VALUES
(1, 'Los Santos', 'ls', 0, 2508.51, -1680.62, 13.5469, 54.8521, 0, 1),
(2, 'San Fierro', 'sf', 0, -2027.06, 172.208, 28.8406, 263.911, 0, 1),
(3, 'Las Venturas', 'lv', 0, 2108.74, 1003.24, 11.1014, 89.8439, 0, 1),
(4, 'Lotnisko LV', 'lvlot', 0, 1670.78, 1597.14, 10.8203, 107.548, 0, 0),
(5, 'Trasa Speed', 'speed', 1, 1633.2, 715.203, 10.8132, 3.51953, 0, 1),
(6, 'DisneyLand', 'disney', 1, 384.128, 621.39, 17.4122, 137.007, 0, 1),
(7, 'Komenda LSPD', 'lspd', 0, 1541.49, -1675.49, 13.5526, 268.547, 0, 0),
(8, 'Molo del Perro', 'molo', 0, 835.556, -1844.16, 12.664, 176.878, 0, 0),
(9, 'Plaza', 'plaza', 0, 334.1, -1813.5, 4.38689, 174.606, 0, 0),
(10, 'Drag Arena', 'drag', 1, -1174.4, -4362.65, 9.37901, 123.326, 0, 1),
(11, 'Lotnisko SF', 'sflot', 0, -1116.52, -201.603, 14.6203, 68.1027, 0, 1),
(12, 'Skocznia', 'skocznia', 1, -1683.32, -2029.82, 152.08, 0.237388, 0, 1),
(13, 'Glen Park', 'park', 0, 1881.99, -1158.21, 23.9912, 238.568, 0, 0),
(14, 'Skate Park', 'skate', 0, 1870.73, -1375.81, 13.5257, 235.067, 0, 0),
(15, 'Lotnisko LS', 'lslot', 0, 1956.3, -2267.69, 13.5469, 179.343, 0, 0),
(16, 'Komenda LV', 'lvpd', 0, 2309.35, 2454, 10.8203, 264.611, 0, 0),
(17, 'Komenda SF', 'sfpd', 0, -1613.97, 663.373, 7.1875, 268.679, 0, 0),
(18, 'Zjazd', 'zjazd', 1, -657.706, -555.702, 1148.86, 89.3845, 0, 1),
(19, 'Pustynia', 'pustynia', 0, 378.485, 2540.95, 16.5391, 180.311, 0, 1),
(20, 'Tama', 'tama', 0, -922.399, 2022.42, 60.9205, 223.447, 0, 1),
(21, 'PGR', 'pgr', 0, 69.8889, -243.974, 1.57812, 1.58412, 0, 0),
(22, 'Doki', 'doki', 0, 2746.9, -2418.69, 13.6282, 235.237, 0, 1),
(23, 'Trasa wokol mapy', 'trasa', 1, -3234.83, -1418.79, 301.384, 186.349, 0, 1),
(24, 'Wojsko', 'wojsko', 0, 299.81, 1979.47, 17.2084, 278.398, 0, 1),
(25, 'Fort Carson', 'carson', 0, -160, 1217.26, 19.31, 184.869, 0, 0),
(26, 'Tuner SF', 'tunesf', 0, -2698.9, 217.792, 3.74748, 89.6558, 0, 0),
(27, 'Kosciol SF', 'kosciolsf', 0, -2730.87, -305.917, 6.7553, 220.819, 0, 1),
(28, 'Drift SF', 'drift1', 0, -2380.9, -588.713, 132.117, 127.679, 0, 1),
(29, 'Mount Chiliad', 'gora', 0, -2323.55, -1629.83, 483.702, 214.278, 0, 1),
(30, 'Angel Pine', 'angelpine', 0, -2122.76, -2489.77, 30.625, 232.695, 0, 1),
(31, 'Most SF', 'most', 0, -2678.92, 1238.08, 55.2496, 12.2162, 0, 0),
(32, 'Port SF', 'portsf', 0, -2336.22, 2315.82, 4.55214, 178.082, 0, 1),
(33, 'Bagno', 'bagno', 0, -852.629, -1937.47, 13.9078, 263.945, 0, 1),
(34, 'Tuner LS', 'tunels', 0, 2644.66, -2026.52, 13.5469, 180.374, 0, 0),
(35, 'Tuner LV', 'tunelv', 0, 2386.75, 1033.81, 10.8203, 358.578, 0, 0),
(36, 'Parking G1', 'g1', 0, 2264.15, 1399.11, 42.3881, 89.8655, 0, 1),
(37, 'Parking G2', 'g2', 0, 2010.64, 1739.24, 18.5017, 2.1197, 0, 0),
(38, 'Parking G3', 'g3', 0, 1681.41, 1175.91, 34.7891, 272.882, 0, 0),
(39, 'Parking G4', 'g4', 0, 2073.75, 2378.76, 49.0969, 182.696, 0, 0),
(40, 'Bialy Dom', 'bialydom', 0, 1133.38, -2037.13, 69.0078, 266.156, 0, 1),
(41, 'Drift LV', 'drift2', 0, -302.407, 1510.46, 74.9272, 181.649, 0, 1),
(42, 'Zamek LV', 'zamek', 1, 2203.89, 1142.06, 85.9375, 286.908, 0, 1),
(43, 'Stunt', 'stunt', 1, 1090.23, 2544.52, 10.3881, 143.524, 0, 1),
(44, 'Trasa gokartowa', 'kart', 1, 2757.83, 2668.62, 10.8331, 353.996, 0, 1),
(45, 'Parking SF', 'parking', 1, -2020.7, 338.76, 35.2, 85.344, 0, 1),
(46, 'Cmentarz LS', 'cmentarz', 0, 949.934, -1103.41, 24.0354, 87.0752, 0, 0),
(47, 'Premium Bank', 'bank', 1, 2421.43, 1123.5, 10.8203, 93.6356, 0, 1),
(48, 'Gielda samochodowa', 'gielda', 1, 1053.53, 1297.84, 10.8323, 357.581, 0, 1),
(49, 'Warehouse', 'wh', 1, 1407.05, -8.15314, 1000.92, 100.967, 1, 0),
(50, 'Urzad Pracy', 'praca', 1, 2639.22, 1993.82, 10.8129, 179.126, 0, 1),
(51, 'Wedkarstwo', 'wedkarstwo', 1, 910.575, 418.556, 20.9761, 17.0604, 0, 1),
(52, 'Trasa Way', 'way', 1, 837.181, -2053.32, 12.8672, 181.113, 0, 1),
(53, 'Szkola jazdy', 'szkola', 1, 2064.84, -1910.47, 13.5601, 89.3475, 0, 1),
(54, 'Kopalnia', 'kopalnia', 1, 436.353, -863.985, 27.7419, 315.953, 0, 1),
(55, 'Dworzec autobusowy', 'PKS', 1, 1009.06, -1364.68, 13.3422, 267.377, 0, 1),
(56, 'Baza spedycyjna', 'tr', 1, 1712.95, 2343.38, 10.3881, 199.348, 0, 1),
(57, 'Baza spedycyjna nr. 2', 'tr2', 1, -28.3573, -2517.24, 36.585, 341.388, 0, 1),
(58, 'Baza spedycyjna nr. 3', 'tr3', 1, 945.726, 1733.26, 8.85156, 268.842, 0, 1);

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `p4s_vehicles`
--

CREATE TABLE `p4s_vehicles` (
  `id` int(11) NOT NULL,
  `modelid` int(11) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `a` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Zrzut danych tabeli `p4s_vehicles`
--

INSERT INTO `p4s_vehicles` (`id`, `modelid`, `x`, `y`, `z`, `a`) VALUES
(1, 600, 2473.46, -1694.78, 13.234, 0.119949),
(2, 492, 2509.38, -1672.23, 13.1819, 348.001),
(3, 481, 2496.64, -1644.04, 13.297, 151.977),
(4, 411, 2132.29, 1013.02, 10.5572, 269.736),
(5, 415, 2129.11, 988.125, 10.6024, 178.777),
(6, 541, 2162.46, 1016.2, 10.4552, 270.289),
(7, 522, 2142.45, 1028.96, 10.3977, 90.5177),
(8, 562, 2179.35, 987.699, 10.489, 0.331621),
(9, 411, -617.227, -542.262, 1148.58, 88.9329),
(10, 541, -617.326, -547.09, 1148.48, 90.0386),
(11, 558, -617.767, -552.005, 1148.49, 90.6238),
(12, 562, -616.99, -557.156, 1148.52, 88.3342),
(13, 560, -616.932, -562.096, 1148.56, 88.2437),
(14, 402, -616.948, -567.212, 1148.69, 88.609),
(15, 444, -2021.53, 351.592, 35.5713, 90.1999),
(16, 571, -2020.11, 346.554, 34.4839, 89.6071),
(17, 571, -2020.07, 343.525, 34.4837, 91.096),
(18, 507, -2082, 401.225, 36.7434, 269.393),
(19, 547, -2081.53, 383.31, 35.518, 269.815),
(20, 534, -2072.3, 418.835, 34.9163, 89.1576),
(21, 445, -2041.66, 429.666, 35.0626, 269.462),
(22, 542, -2021.23, 408.119, 34.9282, 90.2884),
(23, 535, -2110.07, 465.497, 40.795, 90.2331),
(24, 566, -2081.36, 461.784, 40.5757, 270.601),
(25, 566, -2077.77, 359.136, 41.3673, 180.948),
(26, 429, -2047.61, 359.887, 41.2657, 0.435437),
(27, 451, -2021.84, 440.323, 43.4643, 270.532),
(28, 546, -2051.23, 415.549, 45.0874, 90.4568),
(29, 474, -2041.29, 358.974, 47.7547, 179.623),
(30, 480, -2111.54, 386.661, 48.5639, 268.101),
(31, 496, -2080.38, 422.339, 50.785, 90.2544),
(32, 411, -2111.84, 465.554, 53.5556, 271.443),
(33, 565, -2080.71, 468.769, 53.6584, 91.6249);

--
-- Indeksy dla zrzutów tabel
--

--
-- Indeksy dla tabeli `p4s_accounts`
--
ALTER TABLE `p4s_accounts`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_bans`
--
ALTER TABLE `p4s_bans`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `userid` (`userid`);

--
-- Indeksy dla tabeli `p4s_business`
--
ALTER TABLE `p4s_business`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_config`
--
ALTER TABLE `p4s_config`
  ADD PRIMARY KEY (`records`);

--
-- Indeksy dla tabeli `p4s_fun_rc`
--
ALTER TABLE `p4s_fun_rc`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_fun_rc_cp`
--
ALTER TABLE `p4s_fun_rc_cp`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_fun_wg`
--
ALTER TABLE `p4s_fun_wg`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_fun_wp`
--
ALTER TABLE `p4s_fun_wp`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- Indeksy dla tabeli `p4s_gangs`
--
ALTER TABLE `p4s_gangs`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_gangs_bases`
--
ALTER TABLE `p4s_gangs_bases`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_gangs_basesObj`
--
ALTER TABLE `p4s_gangs_basesObj`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`);

--
-- Indeksy dla tabeli `p4s_gangs_zones`
--
ALTER TABLE `p4s_gangs_zones`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_privcars`
--
ALTER TABLE `p4s_privcars`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_privcars_attachments`
--
ALTER TABLE `p4s_privcars_attachments`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_teleports`
--
ALTER TABLE `p4s_teleports`
  ADD PRIMARY KEY (`id`);

--
-- Indeksy dla tabeli `p4s_vehicles`
--
ALTER TABLE `p4s_vehicles`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT dla zrzuconych tabel
--

--
-- AUTO_INCREMENT dla tabeli `p4s_accounts`
--
ALTER TABLE `p4s_accounts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT dla tabeli `p4s_bans`
--
ALTER TABLE `p4s_bans`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT dla tabeli `p4s_business`
--
ALTER TABLE `p4s_business`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT dla tabeli `p4s_fun_rc`
--
ALTER TABLE `p4s_fun_rc`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT dla tabeli `p4s_fun_rc_cp`
--
ALTER TABLE `p4s_fun_rc_cp`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT dla tabeli `p4s_fun_wg`
--
ALTER TABLE `p4s_fun_wg`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT dla tabeli `p4s_fun_wp`
--
ALTER TABLE `p4s_fun_wp`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT dla tabeli `p4s_gangs`
--
ALTER TABLE `p4s_gangs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT dla tabeli `p4s_gangs_bases`
--
ALTER TABLE `p4s_gangs_bases`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT dla tabeli `p4s_gangs_basesObj`
--
ALTER TABLE `p4s_gangs_basesObj`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT dla tabeli `p4s_gangs_zones`
--
ALTER TABLE `p4s_gangs_zones`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=42;

--
-- AUTO_INCREMENT dla tabeli `p4s_privcars`
--
ALTER TABLE `p4s_privcars`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT dla tabeli `p4s_privcars_attachments`
--
ALTER TABLE `p4s_privcars_attachments`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT dla tabeli `p4s_teleports`
--
ALTER TABLE `p4s_teleports`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=59;

--
-- AUTO_INCREMENT dla tabeli `p4s_vehicles`
--
ALTER TABLE `p4s_vehicles`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=35;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
