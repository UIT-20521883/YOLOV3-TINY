#include <stdio.h>
#include <math.h>

#define MAX_ITER 100
#define EPSILON 1e-6

// Hàm tính giá trị của đa thức bậc 5
double f(double x, double a, double b, double c, double d, double e, double f_value)
{ // Đổi tên biến f thành f_value
    return a * pow(x, 5) + b * pow(x, 4) + c * pow(x, 3) + d * pow(x, 2) + e * x + f_value;
}

// Hàm tính đạo hàm của đa thức bậc 5
double df(double x, double a, double b, double c, double d, double e)
{
    return 5 * a * pow(x, 4) + 4 * b * pow(x, 3) + 3 * c * pow(x, 2) + 2 * d * x + e;
}

// Hàm giải phương trình bậc 5 bằng phương pháp Newton-Raphson
double newtonRaphson(double x0, double a, double b, double c, double d, double e, double f_value)
{ // Đổi tên biến f thành f_value
    for (int i = 0; i < MAX_ITER; i++)
    {
        double fx = f(x0, a, b, c, d, e, f_value); // Sử dụng f_value để gọi hàm f
        double dfx = df(x0, a, b, c, d, e);
        if (fabs(dfx) < EPSILON)
        {
            return x0; // Tránh chia cho 0
        }
        double x1 = x0 - fx / dfx;
        if (fabs(x1 - x0) < EPSILON)
        {
            return x1; // Đã hội tụ
        }
        x0 = x1;
    }
    return x0; // Đã đạt số lần lặp tối đa
}

int main()
{
    double a, b, c, d, e, f_value, x0; // Đổi tên biến f thành f_value
    int choice;

    printf("Giai phuong trinh bac 5 (ax^5 + bx^4 + cx^3 + dx^2 + ex + f = 0)\n");

    do
    {
        printf("\nMenu:\n");
        printf("1. Nhap he so\n");
        printf("2. Nhap gia tri khoi dau\n");
        printf("3. Giai phuong trinh\n");
        printf("4. Thoat\n");
        printf("Lua chon cua ban: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Nhap cac he so a, b, c, d, e, f: ");
            scanf("%lf %lf %lf %lf %lf %lf", &a, &b, &c, &d, &e, &f_value); // Sử dụng f_value để lưu giá trị của hằng số f
            break;
        case 2:
            printf("Nhap gia tri khoi dau x0: ");
            scanf("%lf", &x0);
            break;
        case 3:
        {                                                              // Thêm dấu ngoặc nhọn để tạo khối lệnh cho case 3
            double nghiem = newtonRaphson(x0, a, b, c, d, e, f_value); // Sử dụng f_value để gọi hàm newtonRaphson
            printf("Nghiem gan dung: %lf\n", nghiem);
        }
        break;
        case 4:
            printf("Tam biet!\n");
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 4);

    return 0;
}
// test
// alias pp='function _pp(){ git add . && git commit -m "$1" && git push origin gemm; }; _pp'