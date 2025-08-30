<template>
	<view class="wrap">
		<view class="dev-area">

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">水质</view>
					<image class="dev-logo" src="../../static/TDS.png" mode=""></image>
				</view>
				<view class="dev-data">{{TDS}} ppm</view>
			</view>
			<view class="dev-cart">
				<view class="">
					<view class="dev-name">温度</view>
					<image class="dev-logo" src="../../static/temperature.png" mode=""></image>
				</view>
				<view class="dev-data">{{temperature}} ℃</view>
			</view>
			<view class="dev-cart">
				<view class="">
					<view class="dev-name">水量</view>
					<image class="dev-logo" src="../../static/volume.png" mode=""></image>
				</view>
				<view class="dev-data">{{volume}} mL</view>
			</view>
			

		</view>
	</view>

</template>

<script>
	const {
		createCommonToken
	} = require('@/key.js')
	export default {
		data() {
			return {
				TDS: '',
				temperature: '',
				volume: '',
				token: '',
			}
		},
		onLoad() {
			const params = {
				author_key: '6EHiDnegWYJ8DntoGm9Tv9GZWvpkGsb7wgae2CvpngQdI3BPx7Md4w5UQYsyAzYY',
				version: '2022-05-01',
				user_id: '428892',
			}
			this.token = createCommonToken(params);
		},
		onShow() {
			this.fetchDevData();
			setInterval(()=>{
				this.fetchDevData();
			},3000)
		},
		methods: {
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', //仅为示例，并非真实接口地址。
					method: 'GET',
					data: {
						text: 'uni.request',
						product_id: 'l2jlWwNUn4',
						device_name: 'd2',
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: (res) => {
						console.log(res.data);
						console.log(res.data.data[0].value);
						this.TDS = res.data.data[0].value;
						this.temperature = res.data.data[1].value;
						this.volume = res.data.data[2].value;

					}
				});
			},
			onLedSwitch(event) {
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', //仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: 'DQAy602I7Q',
						device_name: 'd1',
						params: {
							"led": value
						}
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: () => {
						console.log('LED ' + (value ? 'ON' : 'OFF') + ' !');
					}
				});
			}

		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx;
	}

	.dev-area {
		display: flex;
		justify-content: space-between;
		flex-wrap: wrap;
	}

	.dev-cart {
		height: 150rpx;
		width: 320rpx;
		border-radius: 30rpx;
		margin-top: 50rpx;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
	}

	.dev-name {
		font-size: 20rpx;
		text-align: center;
		color: #6d6d6d;
	}

	.dev-logo {
		width: 70rpx;
		height: 70rpx;
		margin-top: 10rpx;
	}

	.dev-data {
		font-size: 50rpx;
		color: #6d6d6d;
	}

</style>