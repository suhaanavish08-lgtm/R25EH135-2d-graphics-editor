#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 60
#define MAX_OBJECTS 50

typedef enum { LINE, RECTANGLE, TRIANGLE, CIRCLE } ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} GeometricObject;

char canvas[ROWS][COLS];
GeometricObject object_list[MAX_OBJECTS];
int object_count = 0;
int next_id = 1;

void clear_canvas() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            canvas[r][c] = '_';
        }
    }
}

void draw_pixel(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = '*';
    }
}

void plot_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        draw_pixel(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void plot_circle_points(int xc, int yc, int x, int y) {
    draw_pixel(xc + x, yc + y);
    draw_pixel(xc - x, yc + y);
    draw_pixel(xc + x, yc - y);
    draw_pixel(xc - x, yc - y);
    draw_pixel(xc + y, yc + x);
    draw_pixel(xc - y, yc + x);
    draw_pixel(xc + y, yc - x);
    draw_pixel(xc - y, yc - x);
}

void plot_circle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    plot_circle_points(xc, yc, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        plot_circle_points(xc, yc, x, y);
    }
}

void plot_rectangle(int x1, int y1, int x2, int y2) {
    plot_line(x1, y1, x2, y1);
    plot_line(x1, y2, x2, y2);
    plot_line(x1, y1, x1, y2);
    plot_line(x2, y1, x2, y2);
}

void plot_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    plot_line(x1, y1, x2, y2);
    plot_line(x2, y2, x3, y3);
    plot_line(x3, y3, x1, y1);
}

void refresh_canvas_matrix() {
    clear_canvas();
    for (int i = 0; i < object_count; i++) {
        GeometricObject obj = object_list[i];
        if (obj.type == LINE) plot_line(obj.x1, obj.y1, obj.x2, obj.y2);
        else if (obj.type == RECTANGLE) plot_rectangle(obj.x1, obj.y1, obj.x2, obj.y2);
        else if (obj.type == TRIANGLE) plot_triangle(obj.x1, obj.y1, obj.x2, obj.y2, obj.x3, obj.y3);
        else if (obj.type == CIRCLE) plot_circle(obj.x1, obj.y1, obj.radius);
    }
}

void display_picture() {
    refresh_canvas_matrix();
    printf("\n   ");
    for (int c = 0; c < COLS; c++) {
        if (c % 10 == 0) printf("%d", c / 10);
        else printf(" ");
    }
    printf("\n   ");
    for (int c = 0; c < COLS; c++) printf("%d", c % 10);
    printf("\n");

    for (int r = 0; r < ROWS; r++) {
        printf("%02d ", r);
        for (int c = 0; c < COLS; c++) {
            printf("%c", canvas[r][c]);
        }
        printf("\n");
    }
}

void add_object() {
    if (object_count >= MAX_OBJECTS) {
        printf("Error: Canvas history stack full!\n");
        return;
    }
    GeometricObject new_obj;
    new_obj.id = next_id++;
    printf("\nSelect Shape Type:\n1. Line\n2. Rectangle\n3. Triangle\n4. Circle\nChoice: ");
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) {
        new_obj.type = LINE;
        printf("Enter start (X1 Y1) and end (X2 Y2): ");
        scanf("%d %d %d %d", &new_obj.x1, &new_obj.y1, &new_obj.x2, &new_obj.y2);
    } else if (choice == 2) {
        new_obj.type = RECTANGLE;
        printf("Enter top-left (X1 Y1) and bottom-right (X2 Y2): ");
        scanf("%d %d %d %d", &new_obj.x1, &new_obj.y1, &new_obj.x2, &new_obj.y2);
    } else if (choice == 3) {
        new_obj.type = TRIANGLE;
        printf("Enter Vertices (X1 Y1), (X2 Y2), and (X3 Y3): ");
        scanf("%d %d %d %d %d %d", &new_obj.x1, &new_obj.y1, &new_obj.x2, &new_obj.y2, &new_obj.x3, &new_obj.y3);
    } else if (choice == 4) {
        new_obj.type = CIRCLE;
        printf("Enter Center point (Xc Yc) and Radius: ");
        scanf("%d %d %d", &new_obj.x1, &new_obj.y1, &new_obj.radius);
    } else {
        printf("Invalid configuration skipped.\n");
        return;
    }
    object_list[object_count++] = new_obj;
    printf("Object added successfully with ID: %d\n", new_obj.id);
}

void delete_object() {
    if (object_count == 0) {
        printf("Nothing to remove.\n");
        return;
    }
    int target_id, found_index = -1;
    printf("Enter the Object ID to delete: ");
    scanf("%d", &target_id);
    for (int i = 0; i < object_count; i++) {
        if (object_list[i].id == target_id) {
            found_index = i;
            break;
        }
    }
    if (found_index == -1) {
        printf("Object ID %d not found.\n", target_id);
        return;
    }
    for (int i = found_index; i < object_count - 1; i++) {
        object_list[i] = object_list[i + 1];
    }
    object_count--;
    printf("Object %d removed.\n", target_id);
}

void modify_object() {
    if (object_count == 0) {
        printf("No objects to modify.\n");
        return;
    }
    int target_id, found_index = -1;
    printf("Enter Object ID to modify: ");
    scanf("%d", &target_id);
    for (int i = 0; i < object_count; i++) {
        if (object_list[i].id == target_id) {
            found_index = i;
            break;
        }
    }
    if (found_index == -1) {
        printf("Object ID %d not found.\n", target_id);
        return;
    }
    GeometricObject *obj = &object_list[found_index];
    printf("Updating item ID: %d\n", obj->id);
    if (obj->type == LINE || obj->type == RECTANGLE) {
        printf("Enter new structural coordinates (X1 Y1 X2 Y2): ");
        scanf("%d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2);
    } else if (obj->type == TRIANGLE) {
        printf("Enter three new corners (X1 Y1 X2 Y2 X3 Y3): ");
        scanf("%d %d %d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2, &obj->x3, &obj->y3);
    } else if (obj->type == CIRCLE) {
        printf("Enter new center and size (Xc Yc Radius): ");
        scanf("%d %d %d", &obj->x1, &obj->y1, &obj->radius);
    }
    printf("Object ID %d updated successfully.\n", target_id);
}

void list_active_objects() {
    printf("\n--- Current Objects Active ---\n");
    if (object_count == 0) printf("None\n");
    for (int i = 0; i < object_count; i++) {
        GeometricObject o = object_list[i];
        char* type_name = (o.type == LINE) ? "Line" : (o.type == RECTANGLE) ? "Rectangle" : (o.type == TRIANGLE) ? "Triangle" : "Circle";
        printf("[ID: %d] Shape: %s\n", o.id, type_name);
    }
    printf("-------------------------------\n");
}

int main() {
    int choice = 0;
    while (1) {
        printf("\n=== Terminal 2D Vector Canvas Graphics Engine ===\n");
        printf("1. Display Drawing Canvas Picture\n");
        printf("2. Add Shape Object\n");
        printf("3. Delete Shape Object\n");
        printf("4. Modify Existing Shape Coordinates\n");
        printf("5. List Active Elements Stack\n");
        printf("6. Terminate Program\n");
        printf("Selection Menu Route: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: display_picture(); break;
            case 2: add_object(); display_picture(); break;
            case 3: delete_object(); display_picture(); break;
            case 4: modify_object(); display_picture(); break;
            case 5: list_active_objects(); break;
            case 6: printf("Exiting Program.\n"); return 0;
            default: printf("Option unsupported.\n");
        }
    }
    return 0;
}