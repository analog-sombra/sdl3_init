#!/usr/bin/env python3
"""
Line counter for the Vulkan Day1 project
Counts lines in all files excluding this script itself
"""

import os
from pathlib import Path
from collections import defaultdict

def count_lines(file_path):
    """Count the number of lines in a file."""
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            return len(f.readlines())
    except:
        return 0

def get_file_extension(file_path):
    """Get file extension or 'no extension' for files without one."""
    ext = Path(file_path).suffix
    return ext if ext else 'no extension'

def analyze_project(root_dir='.', exclude_dirs=None):
    """Analyze the project and count lines in all files."""
    if exclude_dirs is None:
        exclude_dirs = {'.git', 'build', '__pycache__', '.vscode', 'node_modules', '.cmake', 'assets'}
    
    file_lines = {}
    extension_stats = defaultdict(lambda: {'count': 0, 'lines': 0})
    total_lines = 0
    total_files = 0
    
    # Get the absolute path of this script to exclude it
    script_path = os.path.abspath(__file__)
    
    for root, dirs, files in os.walk(root_dir):
        # Remove excluded directories from dirs to prevent os.walk from traversing them
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        
        for file in files:
            file_path = os.path.join(root, file)
            abs_path = os.path.abspath(file_path)
            
            # Skip the script itself
            if abs_path == script_path:
                continue
            
            lines = count_lines(file_path)
            file_lines[file_path] = lines
            
            ext = get_file_extension(file_path)
            extension_stats[ext]['count'] += 1
            extension_stats[ext]['lines'] += lines
            
            total_lines += lines
            total_files += 1
    
    return file_lines, extension_stats, total_lines, total_files

def print_report(file_lines, extension_stats, total_lines, total_files):
    """Print a formatted report of line counts."""
    print("\n" + "=" * 80)
    print("PROJECT LINE COUNT ANALYSIS")
    print("=" * 80)
    
    # Print file-by-file breakdown
    print("\n📄 FILES AND LINE COUNTS:")
    print("-" * 80)
    print(f"{'File Path':<50} {'Lines':>10}")
    print("-" * 80)
    
    sorted_files = sorted(file_lines.items(), key=lambda x: x[1], reverse=True)
    for file_path, lines in sorted_files:
        # Use relative path for readability
        rel_path = os.path.relpath(file_path)
        print(f"{rel_path:<50} {lines:>10}")
    
    # Print file type statistics
    print("\n" + "=" * 80)
    print("📊 STATISTICS BY FILE TYPE:")
    print("-" * 80)
    print(f"{'File Type':<20} {'Count':>10} {'Total Lines':>15}")
    print("-" * 80)
    
    sorted_extensions = sorted(extension_stats.items(), 
                               key=lambda x: x[1]['lines'], reverse=True)
    for ext, stats in sorted_extensions:
        print(f"{ext:<20} {stats['count']:>10} {stats['lines']:>15}")
    
    # Print summary
    print("\n" + "=" * 80)
    print("📈 SUMMARY:")
    print("-" * 80)
    print(f"Total Files:  {total_files}")
    print(f"Total Lines:  {total_lines:,}")
    print(f"Average Lines per File: {total_lines / total_files if total_files > 0 else 0:.2f}")
    print("=" * 80 + "\n")

if __name__ == "__main__":
    project_root = os.path.dirname(os.path.abspath(__file__))
    os.chdir(project_root)
    
    print("Analyzing project...")
    file_lines, extension_stats, total_lines, total_files = analyze_project()
    print_report(file_lines, extension_stats, total_lines, total_files)
