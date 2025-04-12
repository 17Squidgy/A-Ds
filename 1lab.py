import numpy as np
import os

def read_points(filename):
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
    except FileNotFoundError:
        print("Файл не найден. Попробуйте снова.")
        return None, None

    x_vals = []
    y_vals = []

    for line in lines:
        parts = line.strip().split()
        if len(parts) != 2:
            print("Ошибка: строка должна содержать 2 значения (x и y), разделённых пробелом.")
            return None, None
        try:
            x, y = map(float, parts)
            x_vals.append(x)
            y_vals.append(y)
        except ValueError:
            print("Ошибка: значения должны быть числами.")
            return None, None

    if len(x_vals) < 2:
        print("Недостаточно данных для аппроксимации.")
        return None, None

    return np.array(x_vals), np.array(y_vals)

def least_squares_approximation(x, y, degree):
    A = np.vander(x, degree + 1, increasing=True)
    coeffs = np.linalg.lstsq(A, y, rcond=None)[0]
    return coeffs

def main():
    while True:
        filename = input("Введите имя файла с координатами (например, data.txt): ")
        x, y = read_points(filename)
        if x is not None and y is not None:
            break  # выход из цикла, если данные успешно считаны

    max_degree = len(x) - 1
    while True:
        try:
            degree = int(input(f"Введите степень полинома (от 0 до {max_degree}): "))
            if 0 <= degree <= max_degree:
                break
            else:
                print(f"Степень полинома должна быть от 0 до {max_degree}.")
        except ValueError:
            print("Введите целое число.")

    coeffs = least_squares_approximation(x, y, degree)

    print("\nКоэффициенты аппроксимирующего полинома:")
    for i, c in enumerate(coeffs):
        print(f"a{i} = {c:.5f}")

    terms = [f"{coeffs[i]:+0.5f}*x^{i}" for i in range(len(coeffs))]
    polynomial = " ".join(terms)
    print("\nАппроксимирующий полином:")
    print(f"P(x) = {polynomial}")

if __name__ == "__main__":
    main()
