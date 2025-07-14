#!/bin/bash

output_file="combined.txt"
> "$output_file"

# Игнорировать сам выходной файл
ignore_file="$(realpath "$output_file")"

# Рекурсивный поиск файлов с нужными расширениями
find . -type f \( -name "*.pro" -o -name "*.cpp" -o -name "*.h" -o -name "*.ui" \) -print0 | while IFS= read -r -d '' file; do
    # Проверка, не является ли файл выходным
    if [[ "$(realpath "$file")" != "$ignore_file" ]]; then
        echo "Добавление содержимого файла: $file"
        cat "$file" >> "$output_file"
        echo -e "\n\n" >> "$output_file"
    fi
done

echo "Объединение завершено. Содержимое сохранено в $output_file."
