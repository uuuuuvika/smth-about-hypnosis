#!/bin/bash

# GIF Batch Processor
# Processes all GIF files in gifs/ folder and outputs to exported gifs/ folder
# Using 128x32 resolution, 15fps, 3 seconds max


INPUT_FOLDER="/Users/userfriendly/Dropbox/Proyectos/vika-bday-screen/gifs"
OUTPUT_FOLDER="/Users/userfriendly/Dropbox/Proyectos/vika-bday-screen/optimized-gifs"

# Create directories if they don't exist
mkdir -p "$INPUT_FOLDER"
mkdir -p "$OUTPUT_FOLDER"

# Check if gifs directory has any GIF files
gif_count=0
for ext in gif GIF; do
    if ls "$INPUT_FOLDER"/*.$ext >/dev/null 2>&1; then
        gif_count=$((gif_count + 1))
        break
    fi
done

if [ $gif_count -eq 0 ]; then
    echo "No GIF files found in $INPUT_FOLDER/ directory"
    echo "Supported formats: gif, GIF"
    echo "Files in gifs/:"
    ls -la "$INPUT_FOLDER"/ 2>/dev/null || echo "  (directory doesn't exist or is empty)"
    exit 1
fi

# Counter for processed files
processed=0
skipped=0
file_number=1

echo "Starting batch processing..."
echo "Input folder: $INPUT_FOLDER/"
echo "Output folder: $OUTPUT_FOLDER/"
echo "Settings: 128x32, 15fps, 3 seconds max"
echo "----------------------------------------"

# Process each GIF file
for ext in gif GIF; do
    for gif_file in "$INPUT_FOLDER"/*.$ext; do
        # Skip if file doesn't exist (handles case where no files match a pattern)
        [ ! -f "$gif_file" ] && continue
    
        # Get filename without path and extension
        filename=$(basename "$gif_file")
        
        # Output GIF path with sequential numbering
        output_gif="$OUTPUT_FOLDER/${file_number}.gif"
        
        # Check if output already exists
        if [ -f "$output_gif" ]; then
            echo "⚠️  Skipping $filename (${file_number}.gif already exists)"
            ((skipped++))
            ((file_number++))
            continue
        fi
        
        echo "🎬 Processing: $filename → ${file_number}.gif"
        
        # Step 1: Generate palette
        echo "   📊 Generating color palette..."
        if ! ffmpeg -loglevel error -i "$gif_file" \
            -vf "fps=15,scale=128:32:force_original_aspect_ratio=increase,crop=128:32,palettegen" \
            -y palette_temp.png; then
            echo "   ❌ Error generating palette for $filename"
            continue
        fi
        
        # Step 2: Create processed GIF with palette
        echo "   🎨 Creating processed GIF..."
        if ffmpeg -loglevel error -i "$gif_file" -i palette_temp.png \
            -filter_complex "fps=15,scale=128:32:force_original_aspect_ratio=increase,crop=128:32[x];[x][1:v]paletteuse" \
            -y "$output_gif"; then
            
            # Get file sizes for comparison
            original_size=$(du -h "$gif_file" | cut -f1)
            gif_size=$(du -h "$output_gif" | cut -f1)
            
            echo "   ✅ Success: $filename → ${file_number}.gif ($original_size → $gif_size)"
            ((processed++))
            ((file_number++))
        else
            echo "   ❌ Error processing GIF for $filename"
        fi
        
        # Clean up temporary palette file
        rm -f palette_temp.png
        
        echo ""
    done
done

# Summary
echo "----------------------------------------"
echo "Processing complete!"
echo "✅ Processed: $processed files"
echo "⚠️  Skipped: $skipped files"
echo "📁 Output folder: $OUTPUT_FOLDER/"

if [ $processed -eq 0 ]; then
    echo ""
    echo "No files were processed. Check that:"
    echo "1. GIF files exist in $INPUT_FOLDER/ folder"
    echo "2. ffmpeg is installed and accessible"
    echo "3. GIF files are in supported formats (gif, GIF)"
fi