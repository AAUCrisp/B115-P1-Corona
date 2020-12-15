#!/usr/bin/env bash
set -euo pipefail

git pull
sudo ln -s -f Api.php /var/www/html/Api.php
sudo ln -s -f Database.php /var/www/html/Database.php
sudo ln -s -f Index.php /var/www/html/index.php

