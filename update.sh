#!/usr/bin/env bash
set -euo pipefail

git pull
sudo cp Api.php /var/www/html/Api.php
sudo cp Database.php /var/www/html/Database.php
sudo cp Index.php /var/www/html/index.php

