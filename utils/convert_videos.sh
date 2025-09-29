#!/bin/bash

# Video to GIF Batch Converter
# Converts all video files in footage/ folder to GIFs in exports/ folder
# Using 128x32 resolution, 15fps, 3 seconds max

# Create directories if they don't exist
mkdir -p footage
mkdir -p exports

# Check if footage directory has any video files
video_count=0
for ext in mp4 mov avi mkv m4v MP4 MOV AVI MKV M4V; do
    if ls footage/*.$ext >/dev/null 2>&1; then
        video_count=$((video_count + 1))
        break
    fi
done

if [ $video_count -eq 0 ]; then
    echo "No video files found in footage/ directory"
    echo "Supported formats: mp4, mov, avi, mkv, m4v"
    echo "Files in footage/:"
    ls -la footage/ 2>/dev/null || echo "  (directory doesn't exist or is empty)"
    exit 1
fi

# Counter for processed files
processed=0
skipped=0
file_number=1

echo "Starting batch conversion..."
echo "Input folder: footage/"
echo "Output folder: exports/"
echo "Settings: 128x32, 15fps, 3 seconds max"
echo "----------------------------------------"

# Process each video file
for ext in mp4 mov avi mkv m4v MP4 MOV AVI MKV M4V; do
    for video_file in footage/*.$ext; do
        # Skip if file doesn't exist (handles case where no files match a pattern)
        [ ! -f "$video_file" ] && continue
    
        # Get filename without path and extension
        filename=$(basename "$video_file")
        
        # Output GIF path with sequential numbering
        output_gif="exports/${file_number}.gif"
        
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
        if ! ffmpeg -loglevel error -t 3 -i "$video_file" \
            -vf "fps=15,scale=128:32:force_original_aspect_ratio=increase,crop=128:32,palettegen" \
            -y palette_temp.png; then
            echo "   ❌ Error generating palette for $filename"
            continue
        fi
        
        # Step 2: Create GIF with palette
        echo "   🎨 Creating GIF..."
        if ffmpeg -loglevel error -t 3 -i "$video_file" -i palette_temp.png \
            -filter_complex "fps=15,scale=128:32:force_original_aspect_ratio=increase,crop=128:32[x];[x][1:v]paletteuse" \
            -y "$output_gif"; then
            
            # Get file sizes for comparison
            original_size=$(du -h "$video_file" | cut -f1)
            gif_size=$(du -h "$output_gif" | cut -f1)
            
            echo "   ✅ Success: $filename → ${file_number}.gif ($original_size → $gif_size)"
            ((processed++))
            ((file_number++))
        else
            echo "   ❌ Error creating GIF for $filename"
        fi
        
        # Clean up temporary palette file
        rm -f palette_temp.png
        
        echo ""
    done
done

# Summary
echo "----------------------------------------"
echo "Conversion complete!"
echo "✅ Processed: $processed files"
echo "⚠️  Skipped: $skipped files"
echo "📁 Output folder: exports/"

if [ $processed -eq 0 ]; then
    echo ""
    echo "No files were processed. Check that:"
    echo "1. Video files exist in footage/ folder"
    echo "2. ffmpeg is installed and accessible"
    echo "3. Video files are in supported formats (mp4, mov, avi, mkv, m4v)"
fi