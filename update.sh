#!/usr/bin/env bash
set -euo pipefail

git pull
sudo cp Api.php /var/www/html/api.php
sudo cp Database.php /var/www/html/database.php
sudo cp Index.php /var/www/html/index.php
sudo cp php_helper.php /var/www/html/php_helper.php
