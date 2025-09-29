#!/bin/bash

# Video Crop Converter
# Converts all video files from input folder to MP4s in output folder
# Using 128x32 resolution, 15fps, no duration limit

# Configuration - Change these folder names as needed
INPUT_FOLDER="/Users/userfriendly/Dropbox/Proyectos/vika-bday-screen/raw-footage"
OUTPUT_FOLDER="/Users/userfriendly/Dropbox/Proyectos/vika-bday-screen/optimized-videos"

# Create directories if they don't exist
mkdir -p "$INPUT_FOLDER"
mkdir -p "$OUTPUT_FOLDER"

# Check if input directory has any video files
video_count=0
for ext in mp4 mov avi mkv m4v MP4 MOV AVI MKV M4V; do
    if ls "$INPUT_FOLDER"/*.$ext >/dev/null 2>&1; then
        video_count=$((video_count + 1))
        break
    fi
done

if [ $video_count -eq 0 ]; then
    echo "No video files found in $INPUT_FOLDER/ directory"
    echo "Supported formats: mp4, mov, avi, mkv, m4v"
    echo "Files in $INPUT_FOLDER/:"
    ls -la "$INPUT_FOLDER"/ 2>/dev/null || echo "  (directory doesn't exist or is empty)"
    exit 1
fi

# Counter for processed files
processed=0
skipped=0
file_number=1

echo "Starting batch conversion..."
echo "Input folder: $INPUT_FOLDER/"
echo "Output folder: $OUTPUT_FOLDER/"
echo "Settings: 128x32, 15fps, no duration limit"
echo "----------------------------------------"

# Process each video file
for ext in mp4 mov avi mkv m4v MP4 MOV AVI MKV M4V; do
    for video_file in "$INPUT_FOLDER"/*.$ext; do
        # Skip if file doesn't exist (handles case where no files match a pattern)
        [ ! -f "$video_file" ] && continue
    
        # Get filename without path and extension
        filename=$(basename "$video_file")
        
        # Output MP4 path with sequential numbering
        output_video="$OUTPUT_FOLDER/${file_number}.mp4"
        
        # Check if output already exists
        if [ -f "$output_video" ]; then
            echo "⚠️  Skipping $filename (${file_number}.mp4 already exists)"
            ((skipped++))
            ((file_number++))
            continue
        fi
        
        echo "🎬 Processing: $filename → ${file_number}.mp4"
        
        # Convert video with cropping and fps limit (optimized for RGB LED matrix)
        echo "   🎥 Converting video..."
        if ffmpeg -loglevel error -i "$video_file" \
            -vf "scale=128:32:force_original_aspect_ratio=increase,crop=128:32" \
            -c:v libx264 -preset slow -crf 18 -pix_fmt rgb24 \
            -an \
            -y "$output_video"; then
            
            # Get file sizes for comparison
            original_size=$(du -h "$video_file" | cut -f1)
            output_size=$(du -h "$output_video" | cut -f1)
            
            echo "   ✅ Success: $filename → ${file_number}.mp4 ($original_size → $output_size)"
            ((processed++))
            ((file_number++))
        else
            echo "   ❌ Error converting video for $filename"
        fi
        
        echo ""
    done
done

# Summary
echo "----------------------------------------"
echo "Conversion complete!"
echo "✅ Processed: $processed files"
echo "⚠️  Skipped: $skipped files"
echo "📁 Output folder: $OUTPUT_FOLDER/"

if [ $processed -eq 0 ]; then
    echo ""
    echo "No files were processed. Check that:"
    echo "1. Video files exist in $INPUT_FOLDER/ folder"
    echo "2. ffmpeg is installed and accessible"
    echo "3. Video files are in supported formats (mp4, mov, avi, mkv, m4v)"
fi