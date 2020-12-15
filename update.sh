#!/usr/bin/env bash
set -euo pipefail

git pull
sudo cp api.php /var/www/html/api.php
sudo cp database.php /var/www/html/database.php
sudo cp index.php /var/www/html/index.php
