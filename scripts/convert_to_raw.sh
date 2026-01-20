#!/bin/bash

# Video to Raw RGB Converter
# Converts video files to raw RGB24 format for LED matrix playback
# Output: 64x32 resolution, 15fps, raw RGB bytes

# Configuration - Change these folder names as needed
INPUT_FOLDER="../assets/input_videos"
OUTPUT_FOLDER="../assets/output_videos"

# Video settings
WIDTH=64
HEIGHT=32
FPS=15

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

echo "Starting batch conversion to raw RGB..."
echo "Input folder: $INPUT_FOLDER/"
echo "Output folder: $OUTPUT_FOLDER/"
echo "Settings: ${WIDTH}x${HEIGHT}, ${FPS}fps, raw RGB24"
echo "----------------------------------------"

# Process each video file
for ext in mp4 mov avi mkv m4v MP4 MOV AVI MKV M4V; do
    for video_file in "$INPUT_FOLDER"/*.$ext; do
        # Skip if file doesn't exist (handles case where no files match a pattern)
        [ ! -f "$video_file" ] && continue
    
        # Get filename without path and extension
        filename=$(basename "$video_file")
        name_no_ext="${filename%.*}"
        
        # Output raw file path
        output_raw="$OUTPUT_FOLDER/${name_no_ext}.rgb"
        
        # Check if output already exists
        if [ -f "$output_raw" ]; then
            echo "⚠️  Skipping $filename (${name_no_ext}.rgb already exists)"
            ((skipped++))
            ((file_number++))
            continue
        fi
        
        echo "🎬 Processing: $filename → ${name_no_ext}.rgb"
        
        # Convert video to raw RGB
        # - Scale with aspect ratio preservation, then crop to exact size
        # - Output raw RGB24 pixels (no header)
        echo "   🎥 Converting to raw RGB..."
        if ffmpeg -loglevel error -i "$video_file" \
            -vf "fps=${FPS},scale=${WIDTH}:${HEIGHT}:force_original_aspect_ratio=increase,crop=${WIDTH}:${HEIGHT}" \
            -f rawvideo -pix_fmt rgb24 \
            -y "$output_raw"; then
            
            # Get file sizes for comparison
            original_size=$(du -h "$video_file" | cut -f1)
            output_size=$(du -h "$output_raw" | cut -f1)
            
            # Calculate frame count
            frame_size=$((WIDTH * HEIGHT * 3))
            file_bytes=$(stat -f%z "$output_raw" 2>/dev/null || stat -c%s "$output_raw" 2>/dev/null)
            frame_count=$((file_bytes / frame_size))
            duration_sec=$((frame_count / FPS))
            
            echo "   ✅ Success: $filename → ${name_no_ext}.rgb"
            echo "      Size: $original_size → $output_size"
            echo "      Frames: $frame_count (~${duration_sec}s at ${FPS}fps)"
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
echo ""
echo "To use a video, copy the .rgb file to the Pi and rename it to 'video.rgb'"
echo "in the same directory as the 'pro' executable."

if [ $processed -eq 0 ]; then
    echo ""
    echo "No files were processed. Check that:"
    echo "1. Video files exist in $INPUT_FOLDER/ folder"
    echo "2. ffmpeg is installed and accessible"
    echo "3. Video files are in supported formats (mp4, mov, avi, mkv, m4v)"
fi
