package main

import (
	"bytes"
	"context"
	"fmt"
	"io"
	"io/fs"
	"log"
	"os"
	"path"
	"path/filepath"
	"strings"

	"github.com/a-h/templ"
	"github.com/gosimple/slug"
	"github.com/yuin/goldmark"
)

type BlogPost struct {
	Date    string
	Title   string
	Content string
}

func main() {
	output_folder := "static"
	blog_dir := "blog_entries"
	md_ending := ".md"

	blog_posts := []BlogPost{}

	filepath.WalkDir(blog_dir, func(s string, d fs.DirEntry, e error) error {
		if e != nil {
			fmt.Printf("error: %v", e)
			return e
		}
		if filepath.Ext(d.Name()) == md_ending {
			file_content, err := os.ReadFile(s)
			if err != nil {
				log.Fatalf("failed to reading %s : %v", s, err)
			}

			fmt.Printf("Contenst of file %s: %s", s, file_content)

			file_name := filepath.Base(d.Name())
			file_name = strings.TrimSuffix(file_name, filepath.Ext(file_name))
			split_name := strings.Split(file_name, "_")
			blog_post := BlogPost{
				Date:    split_name[0],
				Title:   strings.ReplaceAll(split_name[1], "-", " "),
				Content: string(file_content),
			}

			blog_posts = append(blog_posts, blog_post)

			// Create a page for each post.
			blog_post_index_file_path := path.Join(output_folder, slug.Make(path.Join(blog_post.Date, blog_post.Title)))
			if err := os.MkdirAll(blog_post_index_file_path, 0755); err != nil && err != os.ErrExist {
				log.Fatalf("failed to create dir %q: %v", blog_post_index_file_path, err)
			}

			name := path.Join(blog_post_index_file_path, "index.html")
			f, err := os.Create(name)
			if err != nil {
				log.Fatalf("failed to create output file: %v", err)
			}

			// Convert the markdown to HTML, and pass it to the template.
			var buf bytes.Buffer
			if err := goldmark.Convert([]byte(blog_post.Content), &buf); err != nil {
				log.Fatalf("failed to convert markdown to HTML: %v", err)
			}

			// Create an unsafe component containing raw HTML.
			content := UnsafeComponent(buf.String())

			// Use templ to render the template containing the raw HTML.
			err = blogPage(blog_post.Title, content).Render(context.Background(), f)
			if err != nil {
				log.Fatalf("failed to write output file: %v", err)
			}

		}
		return nil
	})

	f, err := os.Create(path.Join(output_folder, "index.html"))
	if err != nil {
		log.Fatalf("failed to create output file: %v", err)
	}

	err = index("ROOK LANG", blog_posts).Render(context.Background(), f)
	if err != nil {
		log.Fatalf("failed to write output file: %v", err)
	}
}

func UnsafeComponent(html string) templ.Component {
	return templ.ComponentFunc(func(ctx context.Context, w io.Writer) (err error) {
		_, err = io.WriteString(w, html)
		return
	})
}
