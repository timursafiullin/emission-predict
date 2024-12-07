import csv
import matplotlib.pyplot as plt

from typing import Sequence, Union, Optional
from numbers import Real

import abc


class RegressorABC(abc.ABC):
    """
    Интерфейс регрессора.
    """

    @abc.abstractmethod
    def fit(self, abscissa: Sequence[Real], ordinates: Sequence[Real]) -> None:
        """
        Обучает регрессор, используя данные обучающей выборки.

        Args:
            abscissa: последовательность абсцисс точек.
            ordinates: последовательность ординат точек.
        """
        ...

    @abc.abstractmethod
    def predict(self, abscissa: Union[Real, Sequence[Real]]) -> list[Real]:
        """
        Аппроксимирует значение функции в переданных точках.

        Args:
            abscissa: число - одна точка, или последовательность точек,
                в которых необходимо аппроксимировать значение функции.

        Returns:
            Список аппроксимаций.

        Raises:
            RuntimeError, если predict вызван до вызова fit.
        """
        ...


class NonparametricRegressor(RegressorABC):
    NEAREST_LIMIT: int = 1

    _abscissa: Optional[list[Real]]
    _ordinates: Optional[list[Real]]
    _k_nearest_original: int
    _k_nearest: int

    def __init__(self, k_nearest: int = 5) -> None:
        self._k_nearest = round(k_nearest)
        self._k_nearest_original = self._k_nearest

        if self._k_nearest < self.NEAREST_LIMIT:
            raise ValueError("k_nearest must be a natural number")

        self._abscissa = None
        self._ordinates = None

    def fit(self, abscissa: Sequence[Real], ordinates: Sequence[Real]) -> None:
        if not abscissa or not ordinates:
            raise ValueError("sequence must not be empty")

        if len(abscissa) != len(ordinates):
            raise ValueError(f"shape mismatch: {len(abscissa)} != {len(ordinates)}")

        self._abscissa = list(abscissa)
        self._ordinates = list(ordinates)
        self._k_nearest = min(self._k_nearest_original, len(abscissa) - 1)

    def predict(self, abscissa: Union[Real, Sequence[Real]]) -> list:
        if self._abscissa is None or self._ordinates is None:
            raise RuntimeError("fit must be called first")

        if isinstance(abscissa, Real):
            return self._get_approximation(abscissa)

        return [self._get_approximation(abscissa_i) for abscissa_i in abscissa]

    def _get_approximation(self, abscissa: Real) -> float:
        k_nearest_abscissa = sorted(self._abscissa, key=lambda x: abs(x - abscissa))[
            self._k_nearest
        ]
        window_size = abs(abscissa - k_nearest_abscissa)
        weights = self._get_weights(abscissa, window_size)

        numerator = sum(
            map(lambda tup_: tup_[0] * tup_[1], zip(self._ordinates, weights)),
        )
        return numerator / sum(weights)

    def _get_weights(self, abscissa: Real, window_size: float) -> list[float]:
        weights = []

        for abscissa_i in self._abscissa:
            kernel_arg = abs(abscissa - abscissa_i) / window_size
            weight = 0.75 * (1 - kernel_arg**2) if kernel_arg < 1 else 0
            weights.append(weight)

        return weights


CO2_regressor = NonparametricRegressor()
CO2_regressor.fit(range(10, 110, 10), [350, 250, 200, 150, 100, 70, 60, 55, 60, 70])
NOX_regressor = NonparametricRegressor()
NOX_regressor.fit(
    range(10, 110, 10), [0.6, 0.7, 0.7, 0.75, 0.8, 0.9, 1.1, 1.25, 1.4, 1.55]
)
PM_regressor = NonparametricRegressor()
PM_regressor.fit(
    range(10, 110, 10), [0.06, 0.07, 0.07, 0.075, 0.08, 0.09, 0.11, 0.125, 0.14, 0.155]
)
VOC_regressor = NonparametricRegressor()
VOC_regressor.fit(
    range(10, 110, 10),
    [0.09, 0.055, 0.05, 0.04, 0.025, 0.023, 0.02, 0.015, 0.013, 0.01],
)
SO2_regressor = NonparametricRegressor()
SO2_regressor.fit(
    range(10, 110, 10),
    [0.09, 0.055, 0.05, 0.04, 0.025, 0.023, 0.02, 0.015, 0.017, 0.02],
)

with open(
    "dataset/vehicle_emission_dataset.csv",
    "r",
    newline="",
) as raw_dataset:
    with open(
        "dataset/vehicle_emission_dataset_synthetic.csv",
        "w",
        newline="",
    ) as synthetic_dataset:
        reader = csv.reader(raw_dataset)
        writer = csv.writer(synthetic_dataset)
        writer.writerow(reader.__next__())
        for row in reader:
            writer.writerow(row)
            current_speed = float(row[5])
            current_CO2 = float(row[13])
            current_NOX = float(row[14])
            current_PM = float(row[15])
            current_VOC = float(row[16])
            current_SO2 = float(row[17])
            CO2_delta = CO2_regressor.predict(current_speed) - current_CO2
            NOX_delta = NOX_regressor.predict(current_speed) - current_NOX
            PM_delta = PM_regressor.predict(current_speed) - current_PM
            VOC_delta = VOC_regressor.predict(current_speed) - current_VOC
            SO2_delta = SO2_regressor.predict(current_speed) - current_SO2
            for speed in range(0, 100, 10):
                current_row = list(row)
                current_row[5] = speed
                current_row[13] = CO2_regressor.predict(speed) - CO2_delta
                current_row[14] = NOX_regressor.predict(speed) - NOX_delta
                current_row[15] = PM_regressor.predict(speed) - PM_delta
                current_row[16] = VOC_regressor.predict(speed) - VOC_delta
                current_row[17] = SO2_regressor.predict(speed) - SO2_delta
                if (
                    current_row[5] < 0
                    or current_row[13] < 0
                    or current_row[14] < 0
                    or current_row[15] < 0
                    or current_row[16] < 0
                    or current_row[17] < 0
                ):
                    continue
                writer.writerow(current_row)
